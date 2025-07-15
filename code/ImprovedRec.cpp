#include <iostream>
#include <vector>
#include <limits>
#include <algorithm>
#include <chrono>
#include <climits>
#include <bitset>

using namespace std;
using namespace chrono;

const int INF = INT_MAX;

// Compile‐time maximum number of cities
static constexpr int MAXN = 1000;

int n;
vector<vector<int>> cost;
int bestCost = INF;

// Compute a refined lower bound using two smallest outgoing edges per unvisited city
int refinedLowerBound(const bitset<MAXN>& mask,
    int costSoFar,
    int current)
{
    int estimate = costSoFar;

    // If all visited, add cost to return to start
    if ((int)mask.count() == n) {
        return estimate + cost[current][0];
    }

    int sum = 0;
    for (int i = 0; i < n; ++i) {
        if (!mask.test(i)) {
            int firstMin = INF, secondMin = INF;
            for (int j = 0; j < n; ++j) {
                if (i != j && cost[i][j] != 0) {
                    if (cost[i][j] < firstMin) {
                        secondMin = firstMin;
                        firstMin = cost[i][j];
                    }
                    else if (cost[i][j] < secondMin) {
                        secondMin = cost[i][j];
                    }
                }
            }
            sum += (firstMin + secondMin);
        }
    }

    return estimate + sum / 2;
}

// B&B TSP recursion
void tsp(int current,
    bitset<MAXN>& mask,
    int costSoFar)
{
    // If all visited, attempt to close the cycle
    if ((int)mask.count() == n) {
        if (cost[current][0] != 0) {
            bestCost = min(bestCost, costSoFar + cost[current][0]);
        }
        return;
    }

    // Prune by lower bound
    int bound = refinedLowerBound(mask, costSoFar, current);
    if (bound >= bestCost) return;

    // Explore next cities
    for (int next = 0; next < n; ++next) {
        if (!mask.test(next) && cost[current][next] != 0) {
            mask.set(next);
            tsp(next, mask, costSoFar + cost[current][next]);
            mask.reset(next);
        }
    }
}

int main() {

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

    // Check for runtime to not exceed MAXN
    if (n > MAXN) {
        cerr << "Error: n = " << n
            << " exceeds compile-time MAXN = " << MAXN << "\n";
        return 1;
    }

    // Initialize bitset mask with only starting city visited
    bitset<MAXN> mask;
    mask.set(0);

    auto t0 = high_resolution_clock::now();
    tsp(0, mask, 0);
    auto t1 = high_resolution_clock::now();

    cout << "Minimum cost Hamiltonian cycle: " << bestCost << "\n"
        << "Time taken: "
        << duration_cast<duration<double, milli>>(t1 - t0).count()
        << " ms\n";

    return 0;
}
