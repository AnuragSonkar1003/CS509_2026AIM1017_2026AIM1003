#include "csr.hpp"
#include <fstream>

CSRGraph adjlist_to_csr(int V,
                        const std::vector<int> &degree,
                        const std::vector<std::vector<int>> &nbr,
                        const std::vector<std::vector<int>> &wt) {
    CSRGraph g;
    g.V = V;
    g.row_ptr.assign(V + 1, 0);
    for (int i = 0; i < V; i++)
        g.row_ptr[i + 1] = g.row_ptr[i] + degree[i];
    g.E = g.row_ptr[V];

    g.col_idx.resize(g.E);
    g.weight.resize(g.E);
    for (int i = 0; i < V; i++) {
        int base = g.row_ptr[i];
        for (int j = 0; j < degree[i]; j++) {
            g.col_idx[base + j] = nbr[i][j];
            g.weight[base + j]  = wt[i][j];
        }
    }
    return g;
}

bool load_bf_graph(const std::string &path, CSRGraph &g, int &source) {
    std::ifstream in(path);
    if (!in) return false;

    int V, E;
    if (!(in >> V >> E) || V <= 0) return false;

    std::vector<int> degree(V, 0);
    std::vector<std::vector<int>> nbr(V), wt(V);

    for (int line = 0; line < V; line++) {
        int u, d;
        if (!(in >> u >> d) || u < 0 || u >= V) return false;
        degree[u] = d;
        nbr[u].resize(d);
        wt[u].resize(d);
        for (int j = 0; j < d; j++) {
            if (!(in >> nbr[u][j] >> wt[u][j])) return false;
        }
    }

    std::string tok;
    int s;
    if (!(in >> tok) || tok != "SOURCE") return false;
    if (!(in >> s) || s < 0 || s >= V) return false;

    g = adjlist_to_csr(V, degree, nbr, wt);
    source = s;
    return true;
}
