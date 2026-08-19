#ifndef UNDIRECTED_CSR_HPP
#define UNDIRECTED_CSR_HPP

#include <string>
#include <vector>

// CSR representation of an unweighted, undirected graph.
// Every undirected edge is stored as two directed arcs (u->v and v->u),
// so row_ptr[V] equals 2*E, not E. Shared by Triangle Counting,
// Betweenness Centrality, and Connected Components (Sections 7.1/8.1/9.1
// all use the identical adjacency-list file format).
struct UCSRGraph {
    int V = 0;                 // number of vertices
    int E = 0;                 // number of undirected edges (as declared in the file header)
    std::vector<int> row_ptr;  // size V+1
    std::vector<int> col_idx;  // size 2*E (both directions of every edge)
};

// Adjacency-list -> CSR helper, kept separate as required by Section 4.1.
// Also sorts each vertex's neighbour segment ascending, since Section 2.4
// recommends sorted neighbour lists for fast common-neighbour checks.
UCSRGraph adjlist_to_ucsr(int V, const std::vector<int> &degree,
                          const std::vector<std::vector<int>> &nbr);

// Reads the adjacency-list file described in Section 7.1 / 8.1 / 9.1:
//   V E
//   u0 degree neighbor1 neighbor2 ...
//   ...
// No SOURCE line for any of these three algorithms.
// Returns false on any malformed/missing file.
bool load_undirected_graph(const std::string &path, UCSRGraph &g);

#endif
