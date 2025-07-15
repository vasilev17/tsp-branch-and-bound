#include <iostream>
#include <vector>
#include <limits>
#include <algorithm>
#include <climits>
#include <chrono>

using namespace std;
using namespace chrono;

const int INF = INT_MAX;   // A large number from <climits> to represent "infinity"

// Global variables
int bestCost = INF;
int n;
vector<vector<int>> cost;
int globalMinEdge;

// Branch-and-bound recursive function for TSP
void tsp(int current, int count, int costSoFar, vector<bool>& visited) {

    // Check if there is an edge returning from the current city to the start city
    if (count == n) {
        if (cost[current][0] != 0) {
            bestCost = min(bestCost, costSoFar + cost[current][0]);
        }
        return;
    }

    // Compute a lower bound
    int bound = costSoFar + (n - count + 1) * globalMinEdge;
    if (bound >= bestCost) return; // Prune the branch if the bound is not better than the current best

    // Try unvisited from the current position
    for (int i = 0; i < n; i++) {
        if (!visited[i] && cost[current][i] != 0) {  // Proceed if i hasn't been visited
            visited[i] = true;
            tsp(i, count + 1, costSoFar + cost[current][i], visited);
            visited[i] = false; // Backtrack: unmark city i for other permutations
        }
    }
}

int main() {
    
    // Inputs to test with 15 cities
    n = 15;
    cost = {
    {0, 34, 47, 85, 61, 22, 63, 38, 93, 76, 15, 48, 60, 67, 44},
    {34, 0, 59, 64, 39, 46, 80, 77, 50, 42, 26, 73, 85, 58, 30},
    {47, 59, 0, 70, 90, 65, 35, 43, 61, 72, 53, 88, 79, 60, 69},
    {85, 64, 70, 0, 95, 37, 76, 25, 84, 56, 33, 66, 72, 94, 41},
    {61, 39, 90, 95, 0, 50, 74, 69, 57, 33, 22, 80, 47, 59, 73},
    {22, 46, 65, 37, 50, 0, 55, 40, 79, 63, 34, 58, 41, 67, 29},
    {63, 80, 35, 76, 74, 55, 0, 61, 48, 70, 28, 90, 60, 82, 77},
    {38, 77, 43, 25, 69, 40, 61, 0, 87, 58, 35, 76, 88, 79, 52},
    {93, 50, 61, 84, 57, 79, 48, 87, 0, 46, 41, 54, 38, 49, 75},
    {76, 42, 72, 56, 33, 63, 70, 58, 46, 0, 21, 83, 92, 71, 36},
    {15, 26, 53, 33, 22, 34, 28, 35, 41, 21, 0, 57, 40, 50, 19},
    {48, 73, 88, 66, 80, 58, 90, 76, 54, 83, 57, 0, 62, 40, 64},
    {60, 85, 79, 72, 47, 41, 60, 88, 38, 92, 40, 62, 0, 27, 71},
    {67, 58, 60, 94, 59, 67, 82, 79, 49, 71, 50, 40, 27, 0, 68},
    {44, 30, 69, 41, 73, 29, 77, 52, 75, 36, 19, 64, 71, 68, 0}
    };

    // Precompute the global minimum edge cost
    globalMinEdge = INF;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i != j && cost[i][j] < globalMinEdge) {
                globalMinEdge = cost[i][j];
            }
        }
    }

    // Initialize visited array and mark the starting city (index 0)
    vector<bool> visited(n, false);
    visited[0] = true;

    auto start = high_resolution_clock::now();

    // Start the TSP search
    tsp(0, 1, 0, visited);

    auto end = high_resolution_clock::now();

    auto duration = duration_cast<nanoseconds>(end - start);

    // Output
    cout << "Minimum cost Hamiltonian cycle: " << bestCost << endl;
    cout << "Time taken: " << duration.count() / 1000000.0 << " ms" << endl;

    return 0;
}
