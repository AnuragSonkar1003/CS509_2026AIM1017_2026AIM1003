#ifndef CONNECTED_COMPONENTS_HPP
#define CONNECTED_COMPONENTS_HPP

#include <vector>
#include "undirected_csr.hpp"

// Finds connected components via BFS, scanning vertices 0..V-1 and starting
// a new traversal from any unvisited vertex. Component ids are assigned in
// the order components are first discovered, starting at 0 (Section 9.3).
// Fills `component` with V entries and sets `num_components`.
void run_connected_components(const UCSRGraph &g, std::vector<int> &component,
                               int &num_components);

#endif
