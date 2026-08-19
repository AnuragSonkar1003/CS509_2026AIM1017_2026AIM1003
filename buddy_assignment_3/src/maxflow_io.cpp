#include "maxflow_io.hpp"
#include <fstream>
#include <iostream>

bool load_maxflow_graph(const std::string &path, CSRGraph &g, int &source, int &sink) {
    std::ifstream in(path);
    if (!in) {
        std::cerr << "Error: cannot open input file: " << path << "\n";
        return false;
    }

    int V, E;
    if (!(in >> V >> E) || V <= 0) {
        std::cerr << "Error: malformed header in " << path << "\n";
        return false;
    }

    std::vector<int> degree(V, 0);
    std::vector<std::vector<int>> nbr(V), wt(V);

    for (int line = 0; line < V; line++) {
        int u, d;
        if (!(in >> u >> d) || u < 0 || u >= V) {
            std::cerr << "Error: malformed adjacency line in " << path << "\n";
            return false;
        }
        degree[u] = d;
        nbr[u].resize(d);
        wt[u].resize(d);
        for (int j = 0; j < d; j++) {
            if (!(in >> nbr[u][j] >> wt[u][j]) || nbr[u][j] < 0 || nbr[u][j] >= V) {
                std::cerr << "Error: malformed neighbour entry for vertex " << u << "\n";
                return false;
            }
            if (wt[u][j] < 0) {
                std::cerr << "Error: negative capacity on edge from vertex " << u << "\n";
                return false;
            }
        }
    }

    std::string tok;
    if (!(in >> tok) || tok != "SOURCE" || !(in >> source) || source < 0 || source >= V) {
        std::cerr << "Error: missing or invalid SOURCE in " << path << "\n";
        return false;
    }
    if (!(in >> tok) || tok != "SINK" || !(in >> sink) || sink < 0 || sink >= V) {
        std::cerr << "Error: missing or invalid SINK in " << path << "\n";
        return false;
    }
    if (source == sink) {
        std::cerr << "Error: source and sink must be different vertices\n";
        return false;
    }

    g = adjlist_to_csr(V, degree, nbr, wt);   // reused from Assignment 2
    return true;
}
