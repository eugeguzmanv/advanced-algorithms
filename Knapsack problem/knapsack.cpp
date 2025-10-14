#include <iostream>
#include <vector>
#include <algorithm> // For std::max

// Function to solve the 0/1 Knapsack problem
int solveKnapsack(int N, int W, const std::vector<int>& values, const std::vector<int>& weights) {
    // Create a DP table to store the maximum value for subproblems.
    // dp[i][w] will be the maximum value that can be obtained using the
    // first 'i' items with a maximum capacity of 'w'.
    // The table size is (N+1) x (W+1) to handle 0 items and 0 capacity.
    std::vector<std::vector<int>> dp(N + 1, std::vector<int>(W + 1, 0));

    // Build the table dp[][] in a bottom-up manner
    for (int i = 1; i <= N; ++i) {
        for (int w = 1; w <= W; ++w) {
            // Get the value and weight of the current item.
            // i-1 is used because vectors are 0-indexed while our loop for i is 1-indexed.
            int current_value = values[i - 1];
            int current_weight = weights[i - 1];

            // If the current item's weight is more than the current knapsack capacity 'w',
            // we cannot include this item. The solution is the same as the solution for the
            // previous i-1 items.
            if (current_weight > w) {
                dp[i][w] = dp[i - 1][w];
            } else {
                // If we can include the item, we have two choices:
                // 1. Don't include the item: The value would be dp[i-1][w].
                // 2. Include the item: The value would be the item's value + the max value
                //    for the remaining capacity (w - current_weight) with i-1 items.
                // We take the maximum of these two choices.
                dp[i][w] = std::max(dp[i - 1][w], current_value + dp[i - 1][w - current_weight]);
            }
        }
    }

    // The bottom-right cell of the table contains the optimal value for N items
    // with a total capacity of W.
    return dp[N][W];
}

int main() {
    // --- Input ---
    int N;
    std::cin >> N;

    std::vector<int> values(N);
    for (int i = 0; i < N; ++i) {
        std::cin >> values[i];
    }

    std::vector<int> weights(N);
    for (int i = 0; i < N; ++i) {
        std::cin >> weights[i];
    }

    int W;
    std::cin >> W;
    
    // --- Solve and Output ---
    int optimal_gain = solveKnapsack(N, W, values, weights);
    std::cout << optimal_gain << std::endl;

    return 0;
}