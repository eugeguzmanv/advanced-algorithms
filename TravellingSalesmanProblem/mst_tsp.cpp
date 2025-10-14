//Traveling Salesman Problem: Minimmum spanning tree approach
//Diego Alberto Carrillo Castro A01645484
//Eugenio Guzmán Vargas A01621084

#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <climits>
#include <string>
using namespace std;

struct City {
    int id;
    long long x;
    long long y;
};

struct Edge {
    int u, v;
    long long cost; // NINT distance
};

//DSU structure for Kruskal's MST
struct DSU {
    vector<int> parent;
    DSU(int n) {
        parent.resize(n);
        for (int i = 0; i < n; ++i) parent[i] = i;
    }
    int find(int i) {
        if (parent[i] == i) return i;
        return parent[i] = find(parent[i]);
    }
    bool unite(int i, int j) {
        int root_i = find(i);
        int root_j = find(j);
        if (root_i != root_j) {
            parent[root_i] = root_j;
            return true;
        }
        return false;
    }
};

vector<City> cities;
int N; 

long long squared_distance(int i, int j) {
    long long dx = cities[i].x - cities[j].x;
    long long dy = cities[i].y - cities[j].y;
    return dx * dx + dy * dy;
}

//Function to calculate the REQUIRED INTEGER Euclidean Distance (NINT rule)
long long integer_distance(int i, int j) {
    return (long long)round(sqrt((double)squared_distance(i, j)));
}

// Forward declarations for functions defined below so helper can call them
long long calculate_tour_cost(const vector<int>& tour);
void eulerian_walk(int u, const vector<vector<int>>& adj, vector<bool>& visited, vector<int>& tour);

long long mst_tsp_tour_cost() {
    //1. Kruskal's MST algorithm:
    vector<Edge> edges;
    for (int i = 0; i < N; ++i) {
        for (int j = i + 1; j < N; ++j) {
            edges.push_back({i, j, integer_distance(i, j)});
        }
    }

    sort(edges.begin(), edges.end(), [](const Edge& a, const Edge& b) {
        return a.cost < b.cost;
    });

    DSU dsu(N);
    vector<vector<int>> mst_adj(N); //Adjacency list for the MST
    int edges_added = 0;

    for (const auto& edge : edges) {
        if (dsu.unite(edge.u, edge.v)) {
            mst_adj[edge.u].push_back(edge.v);
            mst_adj[edge.v].push_back(edge.u);
            edges_added++;
            if (edges_added == N - 1) break; 
        }
    }

    //2. Eulerian Walk to find the tour (preorder traversal):
    vector<int> tour;
    vector<bool> visited(N, false);
    eulerian_walk(0, mst_adj, visited, tour);

    if (tour.size() != N) {
        // indicate error by returning LLONG_MAX
        return LLONG_MAX;
    }

    //3. Calculate the cost of the resulting tour
    long long mst_cost = calculate_tour_cost(tour);
    return mst_cost;
}

bool read_cities(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Could not open file " << filename << endl;
        return false;
    }
    cities.clear();
    int id; long long x, y; char separator;
    while (file >> id >> x >> y) {
        file.get(separator);
        cities.push_back({id - 1, x, y}); 
    }
    N = cities.size();
    return N > 0;
}

//Function to calculate the total tour cost using integer distances
long long calculate_tour_cost(const vector<int>& tour) {
    if (tour.size() != N) return 0;
    long long total_cost = 0;
    for (int i = 0; i < N; ++i) {
        int u = tour[i];
        int v = tour[(i + 1) % N]; //Return to start
        total_cost += integer_distance(u, v);
    }
    return total_cost;
}

//Eulerian walk and shortcutting
void eulerian_walk(int u, const vector<vector<int>>& adj, vector<bool>& visited, vector<int>& tour) {
    if (!visited[u]) {
        visited[u] = true;
        tour.push_back(u); 
    }

    //Traverse all neighbors
    for (int v : adj[u]) {
        if (!visited[v]) {
            eulerian_walk(v, adj, visited, tour);
        }
    }
}

int main() {
    const string filename = "dsj1000.txt";
    const long long optimal_cost = 18659688;

    if (!read_cities(filename)) {
        return 1;
    }

    vector<City> full_cities = cities;
    vector<int> sizes = {1000, 800, 600, 400, 200, 100, 50, 10};

    for (int target : sizes) {
        if ((int)full_cities.size() < target) {
            cout << "--- MST-TSP (N=" << target << ") ---\n";
            cout << "Skipping: input contains only " << full_cities.size() << " cities." << endl;
            continue;
        }

        // use first `target` cities
        cities.assign(full_cities.begin(), full_cities.begin() + target);
        N = target;

        long long mst_cost = mst_tsp_tour_cost();

        cout << "--- MST-TSP (N=" << N << ") ---\n";
        if (mst_cost == LLONG_MAX) {
            cout << "Error: Eulerian shortcut failed to visit all cities for this N." << endl;
        } else {
            cout << "Tour Cost: " << mst_cost << endl;
            
        }
    }

    return 0;
}
