#ifndef CSR_HPP
#define CSR_HPP

#include <string>
#include <vector>

// Compressed Sparse Row representation of a directed, weighted graph.
// Used for Bellman-Ford. Built from an adjacency-list input file.
// CSR construction is preprocessing and is NEVER included in timed work.
struct CSRGraph {
    int V = 0;                 // number of vertices
    int E = 0;                 // number of directed edges
    std::vector<int> row_ptr;  // size V+1
    std::vector<int> col_idx;  // size E
    std::vector<int> weight;   // size E
};

// Adjacency-list -> CSR helper, kept separate as required by Section 4.1.
CSRGraph adjlist_to_csr(int V,
                        const std::vector<int> &degree,
                        const std::vector<std::vector<int>> &nbr,
                        const std::vector<std::vector<int>> &wt);

// Reads the adjacency-list file described in Section 5.1:
//   V E
//   u0 degree n1 w1 n2 w2 ...
//   ...
//   SOURCE s
// Builds an adjacency list, then converts it to CSR via the helper above.
// Returns false on any malformed/missing file.
bool load_bf_graph(const std::string &path, CSRGraph &g, int &source);

#endif
