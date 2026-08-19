#ifndef MAXFLOW_HPP
#define MAXFLOW_HPP

#include <vector>
#include "csr.hpp"

struct CutEdge {
    int u, v;
    int cap;
};

struct MaxflowResult {
    long long max_flow_value = 0;
    long long min_cut_capacity = 0;
    std::vector<int> source_side;
    std::vector<int> sink_side;
    std::vector<CutEdge> cut_edges;
    double time_ms = 0.0;
};

// Runs Dinic's max-flow algorithm on an already-built CSR graph, then
// extracts the minimum cut from the final residual graph (vertices still
// reachable from `source`). Per the assignment's timing rules, building the
// residual network from the CSR, running Dinic's, and the min-cut
// extraction are ALL part of this call and are timed together.
MaxflowResult maxflow_mincut(const CSRGraph &g, int source, int sink);

#endif
