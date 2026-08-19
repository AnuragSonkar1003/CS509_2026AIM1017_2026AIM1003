#include "triangle_counting.hpp"
#include <algorithm>
#include <set>

// True if v and w are directly connected (binary search w within v's sorted
// neighbour segment).
static bool has_edge(const UCSRGraph &g, int v, int w) {
    int lo = g.row_ptr[v], hi = g.row_ptr[v + 1];
    return std::binary_search(g.col_idx.begin() + lo, g.col_idx.begin() + hi, w);
}

void run_triangle_counting(const UCSRGraph &g, long long &total,
                            bool collect_list,
                            std::vector<std::array<int, 3>> &triangles) {
    long long raw = 0;
    std::set<std::array<int, 3>> unique_triangles; // only used when collect_list

    for (int u = 0; u < g.V; u++) {
        int lo = g.row_ptr[u], hi = g.row_ptr[u + 1];
        for (int i = lo; i < hi; i++) {
            for (int j = i + 1; j < hi; j++) {
                int v = g.col_idx[i], w = g.col_idx[j];
                if (has_edge(g, v, w)) {
                    raw++;
                    if (collect_list) {
                        std::array<int, 3> tri = {u, v, w};
                        std::sort(tri.begin(), tri.end());
                        unique_triangles.insert(tri);
                    }
                }
            }
        }
    }

    total = raw / 3;
    if (collect_list)
        triangles.assign(unique_triangles.begin(), unique_triangles.end());
}
