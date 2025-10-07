// Floyd-Warshall with path reconstruction using a hardcoded adjacency matrix.
// The program contains a sample adjacency matrix (directed, weighted).
// Change the matrix below to match your graph. Then run the program and
// input two node indices i j (1-indexed). The program prints the shortest
// distance and the node sequence from i to j. Prints "No path" if unreachable.

#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    const long long INF = (long long)4e15;

    // --- Hardcoded adjacency matrix for sample graph ---
    // Example: 8 nodes (use the graph from your attachment or any other).
    // adj[u][v] = weight, or INF if no direct edge.
    int n = 8;
    vector<vector<long long>> adj = {
        // 1  2  3  4  5  6  7  8  (columns)
        {0, 1, 7, INF, 3, INF, INF, INF}, // 1
        {INF, 0, INF, 4, 6, INF, INF, 2}, // 2
        {INF, INF, 0, INF, 2, 1, INF, INF},// 3
        {INF, INF, 4, 0, INF, 3, 4, 8},    // 4
        {INF, INF, INF, 2, 0, INF, INF, INF},// 5
        {INF, INF, INF, INF, INF, 0, INF, INF},// 6
        {INF, INF, INF, INF, INF, 5, 0, INF},// 7
        {INF, INF, INF, INF, 9, INF, 6, 0}  // 8
    };

    // Ensure adj is n x n; if you replace adj manually, keep dimensions consistent.
    if ((int)adj.size() != n) {
        cerr << "Error: adj size mismatch with n\n";
        return 1;
    }

    // Initialize dist and next (for path reconstruction)
    vector<vector<long long>> dist(n, vector<long long>(n, INF));
    vector<vector<int>> nxt(n, vector<int>(n, -1));

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (i == j) dist[i][j] = 0, nxt[i][j] = j;
            else if (adj[i][j] < INF) {
                dist[i][j] = adj[i][j];
                nxt[i][j] = j;
            }
        }
    }

    // Floyd–Warshall with path reconstruction
    for (int k = 0; k < n; ++k) {
        for (int i = 0; i < n; ++i) {
            if (dist[i][k] >= INF) continue;
            for (int j = 0; j < n; ++j) {
                if (dist[k][j] >= INF) continue;
                long long nd = dist[i][k] + dist[k][j];
                if (nd < dist[i][j]) {
                    dist[i][j] = nd;
                    nxt[i][j] = nxt[i][k];
                }
            }
        }
    }

    // Read query i j from stdin
    cout << "Enter source and target nodes (1-indexed), e.g. '1 4': \n";
    int si, sj;
    if (!(cin >> si >> sj)) return 0;
    if (si < 1 || si > n || sj < 1 || sj > n) {
        cout << "Invalid node indices. Valid range: 1.." << n << "\n";
        return 0;
    }
    int s = si - 1, t = sj - 1;

    if (dist[s][t] >= INF/2) {
        cout << "No path from " << si << " to " << sj << "\n";
        return 0;
    }

    cout << "Shortest distance from " << si << " to " << sj << " = " << dist[s][t] << "\n";
    // Reconstruct path
    vector<int> path;
    int cur = s;
    while (cur != t) {
        path.push_back(cur + 1);
        cur = nxt[cur][t];
        if (cur == -1) { // should not happen
            cout << "Path reconstruction failed\n";
            return 0;
        }
    }
    path.push_back(t + 1);

    cout << "Path: ";
    for (size_t i = 0; i < path.size(); ++i) {
        if (i) cout << " -> ";
        cout << path[i];
    }
    cout << '\n';

    return 0;
}
