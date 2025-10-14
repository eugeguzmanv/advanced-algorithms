//Traveling salesman problem: Cheapest-Link, MST-TSP, and Repeated Nearest Neighbor
//Diego Alberto Carrillo Castro A01645484
//Eugenio Guzmán Vargas A01621084

//This code embeds three algorithms for solving the TSP problem,
//and runs them for multiple sizes (taking the first N cities from the input file) and
//prints a results table.




#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <climits>
#include <string>
#include <iomanip>
using namespace std;


// Raw city record used to read the input once
struct CityRaw { int id; long long x; long long y; };

// sizes to run
static const vector<int> SIZES = {1000, 800, 600, 400, 200, 100, 50, 10};

//Cheapest Link Approach // Complexity: O(N^2 log N) time, O(N^2) space

struct CL_City {
    int id;
    long long x;
    long long y;
};

struct CL_Edge {
    int u, v;
    long long cost; // NINT distance
};

//DSU structure for cycle detection
struct CL_DSU {
    vector<int> parent;
    CL_DSU(int n) {
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

vector<CL_City> CL_cities;
int CL_N;

long long CL_squared_distance(int i, int j) {
    long long dx = CL_cities[i].x - CL_cities[j].x;
    long long dy = CL_cities[i].y - CL_cities[j].y;
    return dx * dx + dy * dy;
}

//Euclidean Distance (NINT rule)
long long CL_integer_distance(int i, int j) {
    return (long long)round(sqrt((double)CL_squared_distance(i, j)));
}


long long CL_cheapest_link_tour_cost() {
    //1 Generate all possible edges using NINT distance
    vector<CL_Edge> edges;
    for (int i = 0; i < CL_N; ++i) {
        for (int j = i + 1; j < CL_N; ++j) {
            edges.push_back({i, j, CL_integer_distance(i, j)});
        }
    }

    //2 Sort edges by cost (ascending)
    sort(edges.begin(), edges.end(), [](const CL_Edge& a, const CL_Edge& b) {
        return a.cost < b.cost;
    });

    CL_DSU dsu(CL_N);
    vector<int> degree(CL_N, 0);
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
            if (dsu.find(u) != dsu.find(v) || edges_added == CL_N - 1) {

                //Add the edge
                dsu.unite(u, v);
                degree[u]++;
                degree[v]++;
                total_cost += edge.cost;
                edges_added++;

                if (edges_added == CL_N) {
                    break;
                }
            }
        }
    }

    return total_cost;
}

// Helper to set cities from raw input (take first `count` entries)
void CL_set_cities_from_raw(const vector<CityRaw>& raw, int count) {
    CL_cities.clear();
    for (int i = 0; i < count; ++i) {
        CityRaw r = raw[i];
        CL_cities.push_back({r.id - 1, r.x, r.y});
    }
    CL_N = (int)CL_cities.size();
}

// Minimmum spanning tree approach // Complexity: O(N^2 log N) time, O(N^2) space

struct MST_City {
    int id;
    long long x;
    long long y;
};

struct MST_Edge {
    int u, v;
    long long cost; // NINT distance
};

//DSU structure for Kruskal's MST
struct MST_DSU {
    vector<int> parent;
    MST_DSU(int n) {
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

vector<MST_City> MST_cities;
int MST_N;

long long MST_squared_distance(int i, int j) {
    long long dx = MST_cities[i].x - MST_cities[j].x;
    long long dy = MST_cities[i].y - MST_cities[j].y;
    return dx * dx + dy * dy;
}

//Function to calculate the REQUIRED INTEGER Euclidean Distance (NINT rule)
long long MST_integer_distance(int i, int j) {
    return (long long)round(sqrt((double)MST_squared_distance(i, j)));
}

// Forward declarations for functions defined below so helper can call them
long long MST_calculate_tour_cost(const vector<int>& tour);
void MST_eulerian_walk(int u, const vector<vector<int>>& adj, vector<bool>& visited, vector<int>& tour);


long long MST_mst_tsp_tour_cost() {
    //1. Kruskal's MST algorithm:
    vector<MST_Edge> edges;
    for (int i = 0; i < MST_N; ++i) {
        for (int j = i + 1; j < MST_N; ++j) {
            edges.push_back({i, j, MST_integer_distance(i, j)});
        }
    }

    sort(edges.begin(), edges.end(), [](const MST_Edge& a, const MST_Edge& b) {
        return a.cost < b.cost;
    });

    MST_DSU dsu(MST_N);
    vector<vector<int>> mst_adj(MST_N); //Adjacency list for the MST
    int edges_added = 0;

    for (const auto& edge : edges) {
        if (dsu.unite(edge.u, edge.v)) {
            mst_adj[edge.u].push_back(edge.v);
            mst_adj[edge.v].push_back(edge.u);
            edges_added++;
            if (edges_added == MST_N - 1) break; 
        }
    }

    //2. Eulerian Walk to find the tour (preorder traversal):
    vector<int> tour;
    vector<bool> visited(MST_N, false);
    MST_eulerian_walk(0, mst_adj, visited, tour);

    if (tour.size() != MST_N) {
        // indicate error by returning LLONG_MAX
        return LLONG_MAX;
    }

    //3. Calculate the cost of the resulting tour
    long long mst_cost = MST_calculate_tour_cost(tour);
    return mst_cost;
}

bool MST_read_cities_original(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Could not open file " << filename << endl;
        return false;
    }
    MST_cities.clear();
    int id; long long x, y; char separator;
    while (file >> id >> x >> y) {
        file.get(separator);
        MST_cities.push_back({id - 1, x, y}); 
    }
    MST_N = MST_cities.size();
    return MST_N > 0;
}

//Function to calculate the total tour cost using integer distances
long long MST_calculate_tour_cost(const vector<int>& tour) {
    if (tour.size() != MST_N) return 0;
    long long total_cost = 0;
    for (int i = 0; i < MST_N; ++i) {
        int u = tour[i];
        int v = tour[(i + 1) % MST_N]; //Return to start
        total_cost += MST_integer_distance(u, v);
    }
    return total_cost;
}

//Eulerian walk and shortcutting
void MST_eulerian_walk(int u, const vector<vector<int>>& adj, vector<bool>& visited, vector<int>& tour) {
    if (!visited[u]) {
        visited[u] = true;
        tour.push_back(u); 
    }

    //Traverse all neighbors
    for (int v : adj[u]) {
        if (!visited[v]) {
            MST_eulerian_walk(v, adj, visited, tour);
        }
    }
}

// Helper to set cities from raw input (take first `count` entries)
void MST_set_cities_from_raw(const vector<CityRaw>& raw, int count) {
    MST_cities.clear();
    for (int i = 0; i < count; ++i) {
        CityRaw r = raw[i];
        MST_cities.push_back({r.id - 1, r.x, r.y});
    }
    MST_N = (int)MST_cities.size();
}

//Nearest neighbour approach // Complexity: O(N^3) time for repeated NN (O(N^2) for single run), O(N) space

struct NN_City {
    int id;
    long long x;
    long long y;
};

vector<NN_City> NN_cities;
int NN_N; 

long long NN_squared_distance(int i, int j) {
    long long dx = NN_cities[i].x - NN_cities[j].x;
    long long dy = NN_cities[i].y - NN_cities[j].y;
    return dx * dx + dy * dy;
}

//Euclidean Distance (NINT rule)
long long NN_integer_distance(int i, int j) {
    return (long long)round(sqrt((double)NN_squared_distance(i, j)));
}

bool NN_read_cities_original(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Could not open file " << filename << endl;
        return false;
    }

    NN_cities.clear();
    int id;
    long long x, y;
    char separator;
    
    //Read format: (# of city) (x_coord) (y_coord) [optional separator]
    while (file >> id >> x >> y) {
        file.get(separator); 
        NN_cities.push_back({id - 1, x, y});
    }

    NN_N = NN_cities.size();
    return NN_N > 0;
}

//Function to calculate the total tour cost using integer distances
long long NN_calculate_tour_cost(const vector<int>& tour) {
    if (tour.size() != NN_N) return LLONG_MAX;
    long long total_cost = 0;
    for (int i = 0; i < NN_N; ++i) {
        int u = tour[i];
        int v = tour[(i + 1) % NN_N]; // Return to start
        total_cost += NN_integer_distance(u, v);
    }
    return total_cost;
}

//Finds a tour starting at a specific city 'start_city'
long long NN_find_nn_tour(int start_city, vector<int>& tour_out) {
    tour_out.clear();
    vector<bool> visited(NN_N, false);
    int current_city = start_city; 
    
    tour_out.push_back(current_city);
    visited[current_city] = true;

    for (int i = 0; i < NN_N - 1; ++i) {
        int nearest_city = -1;
        long long min_dist = LLONG_MAX; 

        for (int next_city = 0; next_city < NN_N; ++next_city) {
            if (!visited[next_city]) {
                long long dist = NN_integer_distance(current_city, next_city);
                
                if (dist < min_dist) {
                    min_dist = dist;
                    nearest_city = next_city;
                }
            }
        }

        if (nearest_city != -1) {
            current_city = nearest_city;
            visited[current_city] = true;
            tour_out.push_back(current_city);
        } else {
            break; 
        }
    }
    
    return NN_calculate_tour_cost(tour_out);
}

// Helper to run repeated NN and return best cost (preserve algorithm logic)
long long NN_run_repeated_nn() {
    long long best_cost = LLONG_MAX;
    vector<int> best_tour;

    for (int start_city = 0; start_city < NN_N; ++start_city) {
        vector<int> current_tour;
        long long current_cost = NN_find_nn_tour(start_city, current_tour);

        if (current_cost < best_cost) {
            best_cost = current_cost;
            best_tour = current_tour;
        }
    }

    return best_cost;
}

// Helper to set cities from raw input (take first `count` entries)
void NN_set_cities_from_raw(const vector<CityRaw>& raw, int count) {
    NN_cities.clear();
    for (int i = 0; i < count; ++i) {
        CityRaw r = raw[i];
        NN_cities.push_back({r.id - 1, r.x, r.y});
    }
    NN_N = (int)NN_cities.size();
}

// -------------------- main --------------------
int main(int argc, char** argv) {
    const string filename = "dsj1000.txt";

    // Read the input once into raw records
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Could not open file " << filename << endl;
        return 1;
    }
    vector<CityRaw> full_cities;
    int id; long long x, y; char separator;
    while (file >> id >> x >> y) {
        file.get(separator);
        full_cities.push_back({id, x, y});
    }

    // Prepare result table: header
    cout << left << setw(8) << "Cities" << setw(20) << "CheapLink" << setw(20) << "MST" << setw(20) << "NN" << '\n';
    cout << string(68, '-') << '\n';

    for (int target : SIZES) {
        if ((int)full_cities.size() < target) {
            cout << left << setw(8) << target << setw(20) << "(missing)" << setw(20) << "(missing)" << setw(20) << "(missing)" << '\n';
            continue;
        }

    // CheapLink
    CL_set_cities_from_raw(full_cities, target);
    long long cheap_cost = CL_cheapest_link_tour_cost();

    // MST
    MST_set_cities_from_raw(full_cities, target);
    long long mst_cost = MST_mst_tsp_tour_cost();

    // NN
    NN_set_cities_from_raw(full_cities, target);
    long long nn_cost = NN_run_repeated_nn();

        auto fmt = [&](long long v)->string {
            if (v == LLONG_MAX) return string("ERR");
            return to_string(v);
        };

        cout << left << setw(8) << target
             << setw(20) << fmt(cheap_cost)
             << setw(20) << fmt(mst_cost)
             << setw(20) << fmt(nn_cost) << '\n';
    }

    return 0;
}
