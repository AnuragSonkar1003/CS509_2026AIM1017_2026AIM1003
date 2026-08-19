#ifndef TRIANGLE_COUNTING_HPP
#define TRIANGLE_COUNTING_HPP

#include <array>
#include <vector>
#include "undirected_csr.hpp"

// Counts triangles using the approach described in Section 2.4: for every
// vertex u, examine each pair of neighbours and check whether that pair is
// itself connected (a common-neighbour check via binary search, since the
// CSR neighbour lists are sorted). Each triangle is found once at each of
// its three vertices, so the raw count is divided by 3.
//
// If `collect_list` is true, every distinct triangle (as a sorted vertex
// triple) is also appended to `triangles` -- intended for the two smallest
// required graph sizes only, per Section 7.3, since the list can get large.
void run_triangle_counting(const UCSRGraph &g, long long &total,
                            bool collect_list,
                            std::vector<std::array<int, 3>> &triangles);

#endif
