//Traveling Salesman Problem: Cheapest-link approach
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

//DSU structure for cycle detection
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

//Euclidean Distance (NINT rule)
long long integer_distance(int i, int j) {
    return (long long)round(sqrt((double)squared_distance(i, j)));
}

// Run the existing Cheapest-Link algorithm on the current global `cities` and `N`.
// Returns the tour cost computed by the algorithm (no changes to the algorithm logic).
long long cheapest_link_tour_cost() {
    //1 Generate all possible edges using NINT distance
    vector<Edge> edges;
    for (int i = 0; i < N; ++i) {
        for (int j = i + 1; j < N; ++j) {
            edges.push_back({i, j, integer_distance(i, j)});
        }
    }

    //2 Sort edges by cost (ascending)
    sort(edges.begin(), edges.end(), [](const Edge& a, const Edge& b) {
        return a.cost < b.cost;
    });

    DSU dsu(N);
    vector<int> degree(N, 0); 
    long long total_cost = 0;
    int edges_added = 0;

   //3 Greedily add edges
    for (const auto& edge : edges) {
        int u = edge.u;
        int v = edge.v;

        //Condition 1: Check degree constraint (must not exceed 2)
        if (degree[u] < 2 && degree[v] < 2) {
            
            //Condition 2: Check for premature cycle formation. 
            //Allow cycle closure only on the N-th edge
            if (dsu.find(u) != dsu.find(v) || edges_added == N - 1) {
                
                //Add the edge
                dsu.unite(u, v);
                degree[u]++;
                degree[v]++;
                total_cost += edge.cost;
                edges_added++;

                if (edges_added == N) {
                    break;
                }
            }
        }
    }

    return total_cost;
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

int main() {
    const string filename = "dsj1000.txt";
    const long long optimal_cost = 18659688;

    if (!read_cities(filename)) {
        return 1;
    }

    // Keep the full list of cities read; we'll take the first M for each experiment
    vector<City> full_cities = cities;

    // sizes to test (first N cities from the input)
    vector<int> sizes = {1000, 800, 600, 400, 200, 100, 50, 10};

    for (int target : sizes) {
        if ((int)full_cities.size() < target) {
            cout << "--- Cheapest-Link TSP (N=" << target << ") ---\n";
            cout << "Skipping: input contains only " << full_cities.size() << " cities." << endl;
            continue;
        }

        // use first `target` cities
        cities.assign(full_cities.begin(), full_cities.begin() + target);
        N = target;

        long long total_cost = cheapest_link_tour_cost();

        cout << "--- Cheapest-Link TSP (N=" << N << ") ---\n";
        cout << "Tour Cost: " << total_cost << endl;
    }

    return 0;
}
