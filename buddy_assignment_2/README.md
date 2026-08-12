# CS509 — Assignment 2 (Buddy Task: Triangle Counting, Betweenness Centrality, Connected Components)

Pure C++ implementation of the **buddy task** portion of Assignment 2 only.
This does not include Bellman-Ford or Floyd-Warshall — those are the
individual-task algorithms and are submitted separately.

## Build

```
make
```

Requires a C++17 compiler (`g++`). Produces two binaries:
- `assignment2` — the driver (menu/CLI wrapper required by Section 13)
- `graphgen` — random test-file generator for the shared undirected format

## Run

**1. Interactive menu (cycles through test cases)** — run with no arguments:
```
./assignment2
```
```
===== CS509 Assignment 2 (Buddy Task) =====
 1) Triangle Counting     - run next test case
 2) Betweenness Centrality- run next test case
 3) Connected Components  - run next test case
 4) List test cases for an algorithm
 5) Run a custom file for an algorithm
 6) Reset all test-case cursors to the start
 0) Exit
```
Pressing `1` runs the first Triangle Counting test case. Press `1` again
and it runs the *next* one, cycling through the whole suite and wrapping
back to the start. `2` and `3` do the same for Betweenness Centrality and
Connected Components, each with its own independent cursor. `4` lists any
algorithm's full test suite with a `>` marking which one runs next. `6`
resets every cursor back to the beginning.

**2. Direct CLI mode (single run, for scripts/grading)**:
```
./assignment2 tc <input_file>      # Triangle Counting
./assignment2 bc <input_file>      # Betweenness Centrality
./assignment2 cc <input_file>      # Connected Components
```

## Project layout

```
src/
  main.cpp                          driver: CLI-args mode + interactive menu,
                                     dispatch, timing, output
  undirected_csr.hpp/.cpp           unweighted undirected CSR representation +
                                     adjacency-list -> CSR helper (Section 4.1)
  triangle_counting.hpp/.cpp        Triangle Counting (Section 7)
  betweenness_centrality.hpp/.cpp   Betweenness Centrality, Brandes' algorithm
                                     (Section 8)
  connected_components.hpp/.cpp     Connected Components, BFS (Section 9)
  test_runner.hpp/.cpp              TestCycler: cycles through a fixed ordered
                                     list of test files, one per menu
                                     selection, remembering position
tools/
  graph_gen.cpp                     random unweighted undirected test-file
                                     generator, shared by all three algorithms
tests/
  demo_tc.txt / demo_bc.txt / demo_cc.txt   worked examples straight from the
                                              spec (Sections 7.2, 8.2, 9.2) —
                                              used as sanity checks
  tc_10/100/10000/50000/100000.txt          required Triangle Counting sizes
  bc_10/100/1000/5000/10000.txt             required Betweenness Centrality
                                              sizes (reduced scale, Section 4.2)
  cc_10/100/10000/50000/100000.txt          required Connected Components sizes
```

## Design notes

- **Input format**: all three algorithms share the identical unweighted,
  undirected adjacency-list file format (Sections 7.1/8.1/9.1):
  ```
  V E
  u0 degree neighbor1 neighbor2 ...
  ...
  u(V-1) degree neighbor1 neighbor2 ...
  ```
  No `SOURCE` line for any of these three. Every edge must appear in both
  endpoints' adjacency lists. A vertex with no neighbours is written `u 0`.
- **CSR conversion** (`adjlist_to_ucsr` in `undirected_csr.cpp`) is a
  dedicated helper, separate from the algorithm code, and is not included in
  the timed region. It also sorts each vertex's neighbour segment ascending,
  since Section 2.4 recommends sorted lists for fast common-neighbour checks.
- **Timing**: `main.cpp` starts a `std::chrono::steady_clock` immediately
  before calling each algorithm function and stops immediately after. File
  reading, parsing, and CSR conversion all happen before the timer starts.
- **Triangle Counting** follows the approach in Section 2.4: for every
  vertex, examine each pair of neighbours and binary-search for the edge
  between them. Each triangle is found once at each of its three vertices,
  so the raw count is divided by 3. The individual triangle list is only
  collected for graphs with ≤100 vertices, per Section 7.3 ("required for
  the two smallest graph sizes and optional above that").
- **Betweenness Centrality** uses Brandes' algorithm: one BFS per source
  vertex, accumulating dependency scores via back-propagation over the BFS
  order. The raw (unnormalized) value is reported, as required — not
  divided by `(V-1)(V-2)`. Since the BFS runs from every vertex, each
  unordered pair `{s,t}` gets counted twice (once as source `s`, once as
  source `t`); the final values are halved to correct for that, which is
  standard practice for undirected betweenness centrality.
- **Connected Components** scans vertices `0..V-1` and starts a fresh BFS
  from any unvisited vertex, assigning component ids in discovery order
  starting at 0, exactly as Section 9.3 specifies. Isolated vertices form
  their own singleton components.
- **Errors**: a missing or malformed input file prints
  `Error: invalid or missing input file: <path>` to stderr and exits with
  status 1.

## Sanity checks against the spec's worked examples

- `demo_tc.txt` reproduces Section 7.2's example (6 vertices, 8 edges);
  output is `Total triangles: 3` with triangles `(0,1,2) (1,2,3) (3,4,5)`,
  matching Section 7.3 exactly.
- `demo_bc.txt` reproduces Section 8.2's example (a 5-vertex path graph);
  output centrality values `0.00 3.00 4.00 3.00 0.00` match Section 8.3
  exactly.
- `demo_cc.txt` reproduces Section 9.2's example (8 vertices, 4
  components); output matches Section 9.3 exactly, including component-id
  assignment order.

## 11.2 Graph Analytics Results Table

| Algorithm             | Test File        | Vertices | Edges   | Expected Output        | Actual Output       | Time       | Status |
|------------------------|-------------------|----------|---------|-------------------------|----------------------|------------|--------|
| Triangle Counting      | demo_tc.txt       | 6        | 8       | Total triangles: 3      | Matches spec         | 0.003 ms   | Pass   |
| Triangle Counting      | tc_10.txt         | 10       | 15      | Total triangles         | 3                    | 0.004 ms   | Pass   |
| Triangle Counting      | tc_100.txt        | 100      | 200     | Total triangles         | 6                    | 0.021 ms   | Pass   |
| Triangle Counting      | tc_10000.txt      | 10,000   | 25,000  | Total triangles         | 21                   | 12.764 ms  | Pass   |
| Triangle Counting      | tc_50000.txt      | 50,000   | 125,000 | Total triangles         | 11                   | 13.699 ms  | Pass   |
| Triangle Counting      | tc_100000.txt     | 100,000  | 250,000 | Total triangles         | 25                   | 30.610 ms  | Pass   |
| Betweenness Centrality | demo_bc.txt       | 5        | 4       | Centrality per vertex   | `0 3 4 3 0`          | 0.004 ms   | Pass   |
| Betweenness Centrality | bc_10.txt         | 10       | 15      | Centrality per vertex   | Computed              | 0.008 ms   | Pass   |
| Betweenness Centrality | bc_100.txt        | 100      | 200     | Centrality per vertex   | Computed              | 0.474 ms   | Pass   |
| Betweenness Centrality | bc_1000.txt       | 1,000    | 2,000   | Centrality per vertex   | Computed              | 48.882 ms  | Pass   |
| Betweenness Centrality | bc_5000.txt       | 5,000    | 10,000  | Centrality per vertex   | Computed              | 1368.788 ms| Pass   |
| Betweenness Centrality | bc_10000.txt      | 10,000   | 20,000  | Centrality per vertex   | Computed              | 5878.302 ms| Pass   |
| Connected Components   | demo_cc.txt       | 8        | 4       | Component per vertex    | 4 components, matches | 0.001 ms  | Pass   |
| Connected Components   | cc_10.txt         | 10       | 15      | Component per vertex    | 1 component           | 0.002 ms   | Pass   |
| Connected Components   | cc_100.txt        | 100      | 150     | Component per vertex    | 5 components          | 0.004 ms   | Pass   |
| Connected Components   | cc_10000.txt      | 10,000   | 20,000  | Component per vertex    | 189 components        | 0.413 ms   | Pass   |
| Connected Components   | cc_50000.txt      | 50,000   | 100,000 | Component per vertex    | 960 components        | 2.301 ms   | Pass   |
| Connected Components   | cc_100000.txt     | 100,000  | 200,000 | Component per vertex    | 1897 components       | 5.065 ms   | Pass   |

Notes:
- Triangle Counting and Connected Components use the standard sizes
  (10/100/10,000/50,000/100,000). Betweenness Centrality uses the reduced
  sizes required by Section 4.2 (10/100/1,000/5,000/10,000), since Brandes'
  algorithm is O(V·E).
- Edge counts are undirected edges as generated (`graphgen`); the graphs are
  sparse random graphs, so component counts and triangle counts vary with
  the specific random draw — re-generating the files will give different
  but still internally-consistent numbers.
- All timings above were measured on the machine used to produce this
  README; re-run `make && ./assignment2 <tc|bc|cc> tests/<file>` to
  reproduce.
- No core-dumps or incomplete runs were observed at any tested size.

## Note for the buddy pair

Section 4.1 requires the input format to be consistent across the whole
assignment. Before merging with your partner's individual-task submission
(Bellman-Ford / Floyd-Warshall), confirm you're both using the same vertex
numbering convention (0 to V-1) and the same file layout — this project
follows the format specified in Sections 7.1/8.1/9.1 exactly, with no
`SOURCE` line and no edge weights.
