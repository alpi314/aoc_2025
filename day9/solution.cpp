#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <set>
#include <cmath>
#include <map>

/*
Could not properly solve this with fast runtime. But leaving this here as a reference implementation.
The key idea was using 4 concepts:
- Compressed grid (instead of storing points we store point reference and interval length to next point, this allows dynamic sizing independent in x and y)
- Grid doubling (this allows easier handling of boundary cases and intersections)
- Inside/Outside test with Ray Casting that also checks for boundary points
- 2D Prefix Sum for fast area checking
*/

using namespace std;

struct Point {
    long long x, y;
    bool operator<(const Point& other) const {
        if (x != other.x) return x < other.x;
        return y < other.y;
    }
};

// Precise Ray Casting
bool isInsideOrBoundary(double x, double y, const vector<Point>& polygon) {
    bool inside = false;
    int n = polygon.size();
    for (int i = 0; i < n; i++) {
        Point p1 = polygon[i];
        Point p2 = polygon[(i + 1) % n];
        
        // Check strict vertical/horizontal boundary
        // We use a small epsilon for double comparison if needed, 
        // but for this logic, strict inequality for ray casting is usually fine.
        // However, we need to be careful about "On Segment".
        
        // Vertical Edge
        if (p1.x == p2.x) {
             if (abs(x - p1.x) < 0.0001) { // On the line
                 if (y >= min(p1.y, p2.y) && y <= max(p1.y, p2.y)) return true;
             }
        }
        // Horizontal Edge
        if (p1.y == p2.y) {
             if (abs(y - p1.y) < 0.0001) { // On the line
                 if (x >= min(p1.x, p2.x) && x <= max(p1.x, p2.x)) return true;
             }
        }

        // Ray Casting
        if (((p1.y > y) != (p2.y > y)) &&
            (x < (p2.x - p1.x) * (y - p1.y) / (double)(p2.y - p1.y) + p1.x)) {
            inside = !inside;
        }
    }
    return inside;
}

int main() {
    ifstream inputFile("input.txt");
    if (!inputFile.is_open()) {
        cerr << "Error: Could not open input.txt" << endl;
        return 1;
    }

    vector<Point> polygon;
    string line;
    while (getline(inputFile, line)) {
        if (line.empty()) continue;
        for (char &c : line) if (c == ',') c = ' ';
        stringstream ss(line);
        long long x, y;
        if (ss >> x >> y) {
            polygon.push_back({x, y});
        }
    }
    inputFile.close();

    if (polygon.empty()) return 0;

    // 1. Collect Unique Coordinates
    vector<long long> Xs, Ys;
    for (const auto& p : polygon) {
        Xs.push_back(p.x);
        Ys.push_back(p.y);
    }
    sort(Xs.begin(), Xs.end());
    Xs.erase(unique(Xs.begin(), Xs.end()), Xs.end());
    sort(Ys.begin(), Ys.end());
    Ys.erase(unique(Ys.begin(), Ys.end()), Ys.end());

    // Map coordinate value -> index in the Expanded Grid
    // Expanded Grid Indices:
    // 0: Xs[0]
    // 1: Interval between Xs[0] and Xs[1]
    // 2: Xs[1]
    // ...
    // Index for Xs[i] = 2*i
    
    auto get_x_idx = [&](long long val) {
        return 2 * (lower_bound(Xs.begin(), Xs.end(), val) - Xs.begin());
    };
    auto get_y_idx = [&](long long val) {
        return 2 * (lower_bound(Ys.begin(), Ys.end(), val) - Ys.begin());
    };

    int rows = 2 * Ys.size() - 1;
    int cols = 2 * Xs.size() - 1;

    // 2. Build Expanded Boolean Grid
    // We use 'int' for prefix sums
    vector<vector<int>> grid(rows, vector<int>(cols));

    for (int r = 0; r < rows; r++) {
        double y_sample;
        if (r % 2 == 0) y_sample = (double)Ys[r/2];
        else y_sample = (Ys[r/2] + Ys[r/2+1]) / 2.0;

        for (int c = 0; c < cols; c++) {
            double x_sample;
            if (c % 2 == 0) x_sample = (double)Xs[c/2];
            else x_sample = (Xs[c/2] + Xs[c/2+1]) / 2.0;

            grid[r][c] = isInsideOrBoundary(x_sample, y_sample, polygon) ? 1 : 0;
        }
    }

    // 3. Build 2D Prefix Sum Array
    // prefix[r][c] = sum of grid[0..r][0..c]
    // We pad with one extra row/col to avoid boundary checks
    vector<vector<int>> prefix(rows + 1, vector<int>(cols + 1, 0));

    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            prefix[r+1][c+1] = prefix[r][c+1] + prefix[r+1][c] - prefix[r][c] + grid[r][c];
        }
    }

    auto get_sum = [&](int r1, int c1, int r2, int c2) {
        return prefix[r2+1][c2+1] - prefix[r1][c2+1] - prefix[r2+1][c1] + prefix[r1][c1];
    };

    // 4. Iterate All Pairs of Vertices
    long long maxArea = 0;
    int n = polygon.size();

    // Loop through every unique pair of vertices
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            Point p1 = polygon[i];
            Point p2 = polygon[j];

            // Determine grid bounds
            int r1 = get_y_idx(min(p1.y, p2.y));
            int r2 = get_y_idx(max(p1.y, p2.y));
            int c1 = get_x_idx(min(p1.x, p2.x));
            int c2 = get_x_idx(max(p1.x, p2.x));

            // Check if this rectangle is fully filled
            // A fully filled rectangle in our grid must have a sum equal to its area (count of cells)
            int expected_count = (r2 - r1 + 1) * (c2 - c1 + 1);
            int actual_count = get_sum(r1, c1, r2, c2);

            if (actual_count == expected_count) {
                // Calculate Physical Area
                long long w = abs(p1.x - p2.x) + 1;
                long long h = abs(p1.y - p2.y) + 1;
                long long area = w * h;
                if (area > maxArea) {
                    maxArea = area;
                }
            }
        }
    }

    cout << "Largest Rectangle Area: " << maxArea << endl;

    return 0;
}