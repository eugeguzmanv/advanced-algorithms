//Traveling Salesman Problem: Nearest neighbour approach
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

bool read_cities(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Could not open file " << filename << endl;
        return false;
    }

    cities.clear();
    int id;
    long long x, y;
    char separator;
    
    //Read format: (# of city) (x_coord) (y_coord) [optional separator]
    while (file >> id >> x >> y) {
        file.get(separator); 
        cities.push_back({id - 1, x, y});
    }

    N = cities.size();
    return N > 0;
}

//Function to calculate the total tour cost using integer distances
long long calculate_tour_cost(const vector<int>& tour) {
    if (tour.size() != N) return LLONG_MAX;
    long long total_cost = 0;
    for (int i = 0; i < N; ++i) {
        int u = tour[i];
        int v = tour[(i + 1) % N]; // Return to start
        total_cost += integer_distance(u, v);
    }
    return total_cost;
}

//Finds a tour starting at a specific city 'start_city'
long long find_nn_tour(int start_city, vector<int>& tour_out) {
    tour_out.clear();
    vector<bool> visited(N, false);
    int current_city = start_city; 
    
    tour_out.push_back(current_city);
    visited[current_city] = true;

    for (int i = 0; i < N - 1; ++i) {
        int nearest_city = -1;
        long long min_dist = LLONG_MAX; 

        for (int next_city = 0; next_city < N; ++next_city) {
            if (!visited[next_city]) {
                long long dist = integer_distance(current_city, next_city);
                
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
    
    return calculate_tour_cost(tour_out);
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
            cout << "--- Repeated Nearest Neighbor (N=" << target << ") ---\n";
            cout << "Skipping: input contains only " << full_cities.size() << " cities." << endl;
            continue;
        }

        cities.assign(full_cities.begin(), full_cities.begin() + target);
        N = target;

        long long best_cost = LLONG_MAX;
        vector<int> best_tour;

        //Run Nearest Neighbor starting from every possible city (0 to N-1)
        for (int start_city = 0; start_city < N; ++start_city) {
            vector<int> current_tour;
            long long current_cost = find_nn_tour(start_city, current_tour);

            if (current_cost < best_cost) {
                best_cost = current_cost;
                best_tour = current_tour;
            }
        }

        long long nn_cost = best_cost;

        cout << "--- Repeated Nearest Neighbor (N=" << N << ") ---\n";
        cout << "Runs performed: " << N << endl;
        if (nn_cost == LLONG_MAX) {
            cout << "Error: a run produced an invalid tour for this N." << endl;
        } else {
            cout << "Best Tour Cost: " << nn_cost << endl;
        }
    }

    return 0;
}
