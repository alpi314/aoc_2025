#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <algorithm>
#include <cmath>
#include <sstream>

using namespace std;

int N = 1000000; // Part1: 1000 (process first 1000), Part2: 1000000 (process all)
int K = 3;

struct Point {
    int x, y, z;
    string key;
};

struct Edge {
    double length;
    string node1;
    string node2;
};

bool compareEdges(const Edge& a, const Edge& b) {
    if (a.length != b.length) {
        return a.length < b.length;
    }
    if (a.node1 != b.node1) {
        return a.node1 < b.node1;
    }
    return a.node2 < b.node2;
}


string parent(const string& node, map<string, string>& parents) {
    if (parents[node] != node) {
        parents[node] = parent(parents[node], parents);
    }
    return parents[node];
}

int main() {
    fstream file("input.txt"); 
    string line;

    vector<Point> points;
    while (std::getline(file, line)) {
        int x, y, z;
        if (sscanf(line.c_str(), "%d,%d,%d", &x, &y, &z) == 3) {
            points.push_back({x, y, z, line});
        }
    }

    vector<Edge> all_edges;
    size_t num_points = points.size();
        for (size_t i = 0; i < num_points; ++i) {
        for (size_t j = i + 1; j < num_points; ++j) {
            const auto& p1 = points[i];
            const auto& p2 = points[j];
            
            double dist = sqrt(
                pow((double)p1.x - p2.x, 2) + 
                pow((double)p1.y - p2.y, 2) + 
                pow((double)p1.z - p2.z, 2)
            );
            
            all_edges.push_back({dist, p1.key, p2.key});
        }
    }
    sort(all_edges.begin(), all_edges.end(), compareEdges);
    
    map<string, string> parents;
    map<string, unsigned long> cluster_sizes;
    int unique_clusters = num_points;
    for (const auto& p : points) {
        parents[p.key] = p.key;
        cluster_sizes[p.key] = 1;
    }

    string last_node1_key = "";
    string last_node2_key = "";

    int i = 0;
    while (i < N && i < all_edges.size()) {
        auto edge = all_edges[i++];
        if (unique_clusters == 1) {
            break;
        }

        string root1 = parent(edge.node1, parents);
        string root2 = parent(edge.node2, parents);

        if (root1 != root2) {            
            last_node1_key = edge.node1;
            last_node2_key = edge.node2;
        
            if (cluster_sizes[root1] < cluster_sizes[root2]) {
                swap(root1, root2);
            }
            
            parents[root2] = root1;
            cluster_sizes[root1] += cluster_sizes[root2];
            cluster_sizes.erase(root2);
            unique_clusters--;
        }
    }

    vector<int> cluster_sizes_vec;
    for (const auto& entry : cluster_sizes) {
        cluster_sizes_vec.push_back(entry.second);
    }
    sort(cluster_sizes_vec.begin(), cluster_sizes_vec.end(), greater<int>());

    int result = 1;
    for (int j = 0; j < min(K, (int)cluster_sizes_vec.size()); ++j) {
        result *= cluster_sizes_vec[j];
    }
    cout << "Result : " << result << endl;

    auto extract_x = [](const string& key) -> int {
        size_t comma_pos = key.find(',');
        return stoi(key.substr(0, comma_pos));
    };

    int x1 = extract_x(last_node1_key);
    int x2 = extract_x(last_node2_key);
    cout << "F(X1 * X2): " << (long long)x1 * x2 << endl;
    return 0;
}