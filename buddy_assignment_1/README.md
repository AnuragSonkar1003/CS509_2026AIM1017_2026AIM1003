# CS509 Assignment 1 - Buddy Part (BFS, DFS, SSSP)

Pair work: BFS, DFS, SSSP, all built on CSR graph storage.

## Files

- driver_buddy.cpp - one program, three modes (bfs, dfs, sssp)
- bfs_10.txt, bfs_100.txt, bfs_10000.txt, bfs_50000.txt, bfs_100000.txt
- dfs_10.txt, dfs_100.txt, dfs_10000.txt, dfs_50000.txt, dfs_100000.txt
- sssp_10.txt, sssp_100.txt, sssp_10000.txt, sssp_50000.txt, sssp_100000.txt

All graph files are built as a random spanning tree plus extra random
edges, so every vertex is reachable from vertex 0. SSSP weights are
random positive integers from 1 to 100.

## Build

```
make
```

```
make clean
```

removes the built binary.

## Run

```
./driver_buddy bfs bfs_10.txt
./driver_buddy dfs dfs_10.txt
./driver_buddy sssp sssp_10.txt
```

Swap the file name to test a different size, e.g.
`./driver_buddy bfs bfs_100000.txt`

Wrong file name or wrong algorithm name prints an error instead of
crashing.

## Graph Results Table

| Algorithm | Test File | Vertices | Edges | Input Type | Source | Expected Output | Actual Output | Time | Status |
|---|---|---|---|---|---|---|---|---|---|
| BFS | bfs_10.txt | 10 | 24 | Unweighted adjacency list | 0 | all vertices reachable | all reachable, 0 unreachable | 0.001 ms | Pass |
| BFS | bfs_100.txt | 100 | 246 | Unweighted adjacency list | 0 | all vertices reachable | all reachable, 0 unreachable | 0.003 ms | Pass |
| BFS | bfs_10000.txt | 10000 | 24997 | Unweighted adjacency list | 0 | all vertices reachable | all reachable, 0 unreachable | 5.08 ms | Pass |
| BFS | bfs_50000.txt | 50000 | 124996 | Unweighted adjacency list | 0 | all vertices reachable | all reachable, 0 unreachable | 2.72 ms | Pass |
| BFS | bfs_100000.txt | 100000 | 249999 | Unweighted adjacency list | 0 | all vertices reachable | all reachable, 0 unreachable | 5.96 ms | Pass |
| DFS | dfs_10.txt | 10 | 24 | Unweighted adjacency list | 0 | valid traversal, all vertices visited | 10 vertices visited | 0.001 ms | Pass |
| DFS | dfs_100.txt | 100 | 249 | Unweighted adjacency list | 0 | valid traversal, all vertices visited | 100 vertices visited | 0.005 ms | Pass |
| DFS | dfs_10000.txt | 10000 | 24996 | Unweighted adjacency list | 0 | valid traversal, all vertices visited | 10000 vertices visited | 1.41 ms | Pass |
| DFS | dfs_50000.txt | 50000 | 124997 | Unweighted adjacency list | 0 | valid traversal, all vertices visited | 50000 vertices visited | 5.67 ms | Pass |
| DFS | dfs_100000.txt | 100000 | 249998 | Unweighted adjacency list | 0 | valid traversal, all vertices visited | 100000 vertices visited | 16.66 ms | Pass |
| SSSP | sssp_10.txt | 10 | 24 | Positive-weighted adjacency list | 0 | all distances finite | all finite, 0 unreachable | 0.002 ms | Pass |
| SSSP | sssp_100.txt | 100 | 249 | Positive-weighted adjacency list | 0 | all distances finite | all finite, 0 unreachable | 0.021 ms | Pass |
| SSSP | sssp_10000.txt | 10000 | 24999 | Positive-weighted adjacency list | 0 | all distances finite | all finite, 0 unreachable | 3.98 ms | Pass |
| SSSP | sssp_50000.txt | 50000 | 124998 | Positive-weighted adjacency list | 0 | all distances finite | all finite, 0 unreachable | 24.53 ms | Pass |
| SSSP | sssp_100000.txt | 100000 | 249998 | Positive-weighted adjacency list | 0 | all distances finite | all finite, 0 unreachable | 48.62 ms | Pass |

Correctness was separately verified against the exact worked examples
in the assignment PDF (Sections 6.2 and 7.2) using a small hand-built
5-vertex graph - BFS gave traversal `0 1 2 3 4`, DFS gave
`0 1 3 2 4`, and SSSP gave distances `0 3 1 4 7`, all matching the
PDF exactly.

## Notes on timing

Timing starts only after the adjacency list is fully loaded and the
CSR arrays (startPos/nbrList) are built, and stops immediately after
the algorithm finishes - CSR conversion, file reading, and printing
are never counted, per Section 8 of the assignment.
