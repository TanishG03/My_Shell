#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;
const int MOD = 998244353;

struct Vertex {
    vector<pair<int, int>> edges;
};

int N;
vector<Vertex> graph;

long long countInversions(int node, int parent, long long parentWeight) {
    long long inversions = 0;

    for (auto edge : graph[node].edges) {
        int child = edge.first;
        int weight = edge.second;
        
        if (child != parent) {
            // Count inversions based on edge weights
            if (weight == 1) {
                inversions += (parentWeight + 1);
            } else {
                inversions += countInversions(child, node, parentWeight + 1);
            }
        }
    }

    return inversions % MOD;
}

int main() {
    cin >> N;
    graph.resize(N + 1); // Vertices are 1-indexed
    
    for (int i = 1; i <= N; ++i) {
        int Si;
        cin >> Si;
        
        for (int j = 0; j < Si; ++j) {
            int Li, Wi;
            cin >> Li >> Wi;
            graph[i].edges.push_back({Li, Wi});
        }
    }

    long long inversions = countInversions(1, -1, 0);
    cout << inversions << endl;

    return 0;
}



// #include <iostream>
// #include <vector>
// #include <cstring>

// using namespace std;
// const int MOD = 998244353;
// const int MAXN = 100005;

// int N;
// vector<pair<int, bool>> graph[MAXN];
// vector<int> Z;
// int inversions = 0;

// void dfs(int i) {
//     for (auto edge : graph[i]) {
//         int j = edge.first;
//         bool w = edge.second;
        
//         Z.push_back(w);
//         dfs(j);
//     }
// }

// int main() {
//     cin >> N;
//     for (int i = 1; i <= N; ++i) {
//         int Si;
//         cin >> Si;
        
//         for (int j = 0; j < Si; ++j) {
//             int Li;
//             bool Wi;
//             cin >> Li >> Wi;
//             graph[i].push_back({Li, Wi});
//         }
//     }

//     dfs(1);

//     int Zsize = Z.size();

//     // for (int i = 0; i < Zsize; ++i) {
//     //     for (int j = i + 1; j < Zsize; ++j) {
//     //         if (Z[i] > Z[j]) {
//     //             inversions++;
//     //             inversions %= MOD;
//     //         }
//     //     }
//     // }
//     int num_zeros = 0;
//      for (int i = Zsize - 1; i >= 0; --i) {
//         if (Z[i] == 0) {
//             num_zeros++;
//         } else {
//             inversions = (inversions + num_zeros) % MOD;
//         }
//     }

//     cout << inversions << endl;
    
//     return 0;
// }
