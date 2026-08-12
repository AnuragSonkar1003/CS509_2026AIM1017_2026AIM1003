#include "betweenness_centrality.hpp"
#include <stack>
#include <queue>

void run_betweenness_centrality(const UCSRGraph &g, std::vector<double> &centrality) {
    int V = g.V;
    centrality.assign(V, 0.0);

    std::vector<long long> sigma(V);      // number of shortest paths from s
    std::vector<int> dist(V);             // BFS distance from s
    std::vector<std::vector<int>> pred(V); // predecessors on shortest paths
    std::vector<double> delta(V);

    for (int s = 0; s < V; s++) {
        std::stack<int> order;            // vertices in non-decreasing distance order
        std::fill(sigma.begin(), sigma.end(), 0);
        std::fill(dist.begin(), dist.end(), -1);
        for (int i = 0; i < V; i++) pred[i].clear();

        sigma[s] = 1;
        dist[s] = 0;
        std::queue<int> bfs;
        bfs.push(s);

        while (!bfs.empty()) {
            int v = bfs.front(); bfs.pop();
            order.push(v);
            for (int idx = g.row_ptr[v]; idx < g.row_ptr[v + 1]; idx++) {
                int w = g.col_idx[idx];
                if (dist[w] < 0) {                 // w visited for the first time
                    dist[w] = dist[v] + 1;
                    bfs.push(w);
                }
                if (dist[w] == dist[v] + 1) {       // shortest path to w goes through v
                    sigma[w] += sigma[v];
                    pred[w].push_back(v);
                }
            }
        }

        std::fill(delta.begin(), delta.end(), 0.0);
        while (!order.empty()) {
            int w = order.top(); order.pop();
            for (int v : pred[w])
                delta[v] += (double)sigma[v] / (double)sigma[w] * (1.0 + delta[w]);
            if (w != s) centrality[w] += delta[w];
        }
    }

    // Each shortest path was counted once from each endpoint's BFS, so every
    // unordered pair {s,t} was counted twice; halve to get the standard
    // undirected betweenness value.
    for (int i = 0; i < V; i++) centrality[i] /= 2.0;
}
