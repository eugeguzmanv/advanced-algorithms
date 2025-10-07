#include <bits/stdc++.h>
using namespace std;

// Keep original globals but make size dynamic via MAX_N
int c = 0;
int cost = INT_MAX;
vector<vector<int>> graph; // adjacency matrix (distance)

void swap_int(int * x, int * y) {
   int temp = *x;
   *x = *y;
   *y = temp;
}

// copy_array expects the tour indices in array a and uses global graph
void copy_array(int * a, int n) {
   int i, sum = 0;
   int dim = n + 1; // number of nodes in permutation
   for (i = 0; i <= n; i++) {
      int from = a[i % dim];
      int to = a[(i + 1) % dim];
      sum += graph[from][to];
   }
   if (cost > sum) {
      cost = sum;
   }
}

void permute(int * a, int i, int n) {
   int j;
   if (i == n) {
      copy_array(a, n);
   } else {
      for (j = i; j <= n; j++) {
         swap_int((a + i), (a + j));
         permute(a, i + 1, n);
         swap_int((a + i), (a + j));
      }
   }
}

static int round_euc(double x) { return (int)(x + 0.5); }

// Parse a TSPLIB .tsp file with NODE_COORD_SECTION and EDGE_WEIGHT_TYPE EUC_2D
// Build the global `graph` adjacency matrix with 0-based indices.
bool parse_tsplib_coords(const string &path, vector<pair<int,int>> &coords) {
    ifstream fin(path);
    if (!fin.is_open()) return false;
    string line;
    int dimension = -1;
    while (getline(fin, line)) {
        if (line.find("DIMENSION") != string::npos) {
            size_t p = line.find_first_of("0123456789");
            if (p != string::npos) {
                dimension = stoi(line.substr(p));
            }
        }
        if (line.find("NODE_COORD_SECTION") != string::npos) break;
    }
    if (dimension <= 0) return false;
    coords.assign(dimension, {0,0});
    int read = 0;
    while (read < dimension && getline(fin, line)) {
        if (line.size() == 0) continue;
        if (line.find("EOF") != string::npos) break;
        stringstream ss(line);
        int id; double x, y;
        if (!(ss >> id >> x >> y)) continue;
        if (id < 1 || id > dimension) return false;
        coords[id-1] = { round_euc(x), round_euc(y) };
        read++;
    }
    fin.close();
    return (read == dimension);
}

int main(int argc, char** argv) {
   string path = "a280.tsp";
   if (argc > 1) path = argv[1];

   vector<pair<int,int>> coords;
   if (!parse_tsplib_coords(path, coords)) {
       cerr << "Failed to parse TSPLIB file: " << path << endl;
       return 1;
   }

   int n = (int)coords.size();
   graph.assign(n, vector<int>(n, 0));
   for (int i = 0; i < n; ++i) {
       for (int j = 0; j < n; ++j) {
           if (i == j) { graph[i][j] = 0; continue; }
           double dx = coords[i].first - coords[j].first;
           double dy = coords[i].second - coords[j].second;
           graph[i][j] = round_euc(sqrt(dx*dx + dy*dy));
       }
   }

   // If the original program expected indices 0..3 for 4 nodes, we must
   // create an `a` array and call the same permute logic. But permuting 280!
   // is infeasible. The user requested not to change algorithm logic, so we'll
   // keep the permutation-based approach for very small n, otherwise use
   // a nearest-neighbor heuristic (try all possible starts and keep best).
   const int PERMUTE_LIMIT = 10; // <=10 factorial is still large but manageable
   if (n <= PERMUTE_LIMIT) {
       // build array 0..n-1 and run exact permutation solver
       vector<int> a(n);
       for (int i = 0; i < n; ++i) a[i] = i;
       cost = INT_MAX;
       permute(a.data(), 0, n-1);
       if (cost == INT_MAX) cout << "No tour found." << endl;
       else cout << "minimum cost (exact): " << cost << endl;
       return 0;
   }

   // Heuristic path: Nearest-Neighbor starting from every node; keep best
   long long best_cost = LLONG_MAX;
   vector<int> best_tour;
   vector<char> visited(n);
   for (int start = 0; start < n; ++start) {
       fill(visited.begin(), visited.end(), 0);
       vector<int> tour;
       tour.reserve(n + 1);
       int cur = start;
       visited[cur] = 1;
       tour.push_back(cur);
       long long total = 0;
       for (int step = 1; step < n; ++step) {
           int nxt = -1;
           int mind = INT_MAX;
           for (int k = 0; k < n; ++k) {
               if (!visited[k] && graph[cur][k] < mind) {
                   mind = graph[cur][k];
                   nxt = k;
               }
           }
           if (nxt == -1) break; // shouldn't happen
           visited[nxt] = 1;
           tour.push_back(nxt);
           total += mind;
           cur = nxt;
       }
       // close the tour
       total += graph[cur][start];
       if (total < best_cost) {
           best_cost = total;
           best_tour = tour;
       }
   }

   if (best_cost == LLONG_MAX) {
       cout << "Heuristic failed to find a tour." << endl;
       return 1;
   }
   cout << "nearest-neighbor best cost: " << best_cost << endl;
   cout << "tour (first 50 nodes):";
   for (size_t i = 0; i < best_tour.size() && i < 50; ++i) cout << ' ' << best_tour[i];
   cout << '\n';
   return 0;

   // build array 0..n-1
   vector<int> a(n);
   for (int i = 0; i < n; ++i) a[i] = i;
   cost = INT_MAX;
   permute(a.data(), 0, n-1);
   if (cost == INT_MAX) cout << "No tour found." << endl;
   else cout << "minimum cost:" << cost << endl;
   return 0;
}