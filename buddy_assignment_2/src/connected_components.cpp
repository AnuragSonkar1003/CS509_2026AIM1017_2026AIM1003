#include "connected_components.hpp"
#include <queue>

void run_connected_components(const UCSRGraph &g, std::vector<int> &component,
                               int &num_components) {
    int V = g.V;
    component.assign(V, -1);
    num_components = 0;

    for (int start = 0; start < V; start++) {
        if (component[start] != -1) continue; // already assigned

        int id = num_components++;
        std::queue<int> bfs;
        bfs.push(start);
        component[start] = id;

        while (!bfs.empty()) {
            int v = bfs.front(); bfs.pop();
            for (int idx = g.row_ptr[v]; idx < g.row_ptr[v + 1]; idx++) {
                int w = g.col_idx[idx];
                if (component[w] == -1) {
                    component[w] = id;
                    bfs.push(w);
                }
            }
        }
    }
}
