#ifndef MAXFLOW_IO_HPP
#define MAXFLOW_IO_HPP

#include <string>
#include "csr.hpp"   // reused, unmodified, from CS509 Assignment 2

// Reads the Section 7.1 directed capacity adjacency-list format:
//   V E
//   u0 degree n1 c1 n2 c2 ...
//   ...
//   SOURCE s
//   SINK t
// Only outgoing edges are listed (directed graph). Builds the per-vertex
// degree/neighbour/capacity vectors and converts them to CSR via
// Assignment 2's adjlist_to_csr() -- reused, not reimplemented.
// Pure file I/O / parsing -- never timed.
//
// Per Section 11, rejects: invalid source/sink vertices, source == sink,
// or negative capacities.
bool load_maxflow_graph(const std::string &path, CSRGraph &g, int &source, int &sink);

#endif
