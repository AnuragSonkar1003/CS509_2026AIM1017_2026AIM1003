#ifndef BETWEENNESS_CENTRALITY_HPP
#define BETWEENNESS_CENTRALITY_HPP

#include <vector>
#include "undirected_csr.hpp"

// Brandes' algorithm for unweighted betweenness centrality (Section 2.5).
// Runs a BFS from every vertex and accumulates, for each vertex v, the
// fraction of shortest paths between other pairs that pass through v.
// Reports the RAW (unnormalized) value -- i.e. it is NOT divided by
// (V-1)(V-2). Fills `centrality` with V entries.
void run_betweenness_centrality(const UCSRGraph &g, std::vector<double> &centrality);

#endif
