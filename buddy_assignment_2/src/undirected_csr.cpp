#include "undirected_csr.hpp"
#include <algorithm>
#include <fstream>

UCSRGraph adjlist_to_ucsr(int V, const std::vector<int> &degree,
                          const std::vector<std::vector<int>> &nbr) {
    UCSRGraph g;
    g.V = V;
    g.row_ptr.assign(V + 1, 0);
    for (int i = 0; i < V; i++)
        g.row_ptr[i + 1] = g.row_ptr[i] + degree[i];

    g.col_idx.resize(g.row_ptr[V]);
    for (int i = 0; i < V; i++) {
        int base = g.row_ptr[i];
        for (int j = 0; j < degree[i]; j++)
            g.col_idx[base + j] = nbr[i][j];
        std::sort(g.col_idx.begin() + base, g.col_idx.begin() + base + degree[i]);
    }
    return g;
}

bool load_undirected_graph(const std::string &path, UCSRGraph &g) {
    std::ifstream in(path);
    if (!in) return false;

    int V, E;
    if (!(in >> V >> E) || V <= 0) return false;

    std::vector<int> degree(V, 0);
    std::vector<std::vector<int>> nbr(V);

    for (int line = 0; line < V; line++) {
        int u, d;
        if (!(in >> u >> d) || u < 0 || u >= V) return false;
        degree[u] = d;
        nbr[u].resize(d);
        for (int j = 0; j < d; j++)
            if (!(in >> nbr[u][j])) return false;
    }

    g = adjlist_to_ucsr(V, degree, nbr);
    g.E = E;
    return true;
}
