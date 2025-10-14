#include <iostream>
#include <vector>
#include <algorithm> // For std::sort

// A structure to hold item details
struct Item {
    int value;
    int weight;
    int original_index; // To keep track if needed, not essential for the final value
};

// Comparison function to sort items by value-to-weight ratio in descending order
bool compareItems(const Item& a, const Item& b) {
    double ratio_a = static_cast<double>(a.value) / a.weight;
    double ratio_b = static_cast<double>(b.value) / b.weight;
    return ratio_a > ratio_b;
}

// Function to solve the Fractional Knapsack problem
double solveFractionalKnapsack(int W, std::vector<Item>& items) {
    // 1. Sort items based on value-to-weight ratio in descending order
    std::sort(items.begin(), items.end(), compareItems);

    double total_value = 0.0;
    int current_weight = 0;

    // 2. Iterate through the sorted items and fill the knapsack
    for (const auto& item : items) {
        // If the knapsack is already full, we can stop
        if (current_weight == W) {
            break;
        }

        // If the entire item fits, take all of it
        if (current_weight + item.weight <= W) {
            current_weight += item.weight;
            total_value += item.value;
        } else {
            // If only a fraction of the item fits, take that fraction
            int remaining_capacity = W - current_weight;
            double fraction = static_cast<double>(remaining_capacity) / item.weight;
            total_value += item.value * fraction;
            current_weight += remaining_capacity; // Knapsack is now full
        }
    }

    return total_value;
}

int main() {
    // --- Input ---
    int N;
    std::cin >> N;

    std::vector<Item> items(N);
    std::vector<int> values(N);
    for (int i = 0; i < N; ++i) {
        std::cin >> values[i];
    }

    std::vector<int> weights(N);
    for (int i = 0; i < N; ++i) {
        std::cin >> weights[i];
    }
    
    // Populate the items vector
    for (int i = 0; i < N; ++i) {
        items[i] = {values[i], weights[i], i};
    }

    int W;
    std::cin >> W;

    // --- Solve and Output ---
    double optimal_gain = solveFractionalKnapsack(W, items);
    
    // Set precision for floating point output
    std::cout.precision(6);
    std::cout << std::fixed << optimal_gain << std::endl;

    return 0;
}