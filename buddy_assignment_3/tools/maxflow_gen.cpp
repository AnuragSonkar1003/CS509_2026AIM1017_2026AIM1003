// Random directed, weighted (capacity) graph generator for Maxflow-Mincut
// test inputs.
//
// Usage: maxflow_gen <V> <E> <output_file> [seed]
//
// Builds a random vertex order, forces source = order[0] and
// sink = order[V-1], and only adds forward edges (earlier -> later in the
// order). This guarantees:
//   - a DAG (no cycles),
//   - a valid source-to-sink path (the chain order[0]->order[1]->...->order[V-1]
//     is added first, before any random extra edges),
// while still producing a sparse, randomly-structured graph.
#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <numeric>
#include <random>
#include <unordered_set>
#include <vector>

int main(int argc, char **argv) {
    if (argc < 4) {
        std::cerr << "Usage: " << argv[0] << " <V> <E> <output_file> [seed]\n";
        return 1;
    }

    long long V = std::atoll(argv[1]);
    long long E = std::atoll(argv[2]);
    std::string outPath = argv[3];
    unsigned seed = argc > 4 ? static_cast<unsigned>(std::atoll(argv[4])) : 7u;

    if (V < 2) {
        std::cerr << "V must be >= 2 (need distinct source and sink)\n";
        return 1;
    }

    long long maxE = V * (V - 1) / 2;   // DAG upper bound (forward edges only)
    if (E < V - 1) E = V - 1;           // must contain the guaranteed path
    if (E > maxE) E = maxE;

    std::mt19937_64 rng(seed);
    std::uniform_int_distribution<long long> capDist(1, 50);

    std::vector<long long> order(V);
    std::iota(order.begin(), order.end(), 0);
    std::shuffle(order.begin(), order.end(), rng);

    long long source = order[0];
    long long sink = order[V - 1];

    std::vector<long long> pos(V);
    for (long long i = 0; i < V; i++) pos[order[i]] = i;

    std::unordered_set<long long> used;
    used.reserve(static_cast<size_t>(E * 2));
    std::vector<std::vector<std::pair<long long, long long>>> adj(V);   // outgoing only

    auto edgeKey = [V](long long a, long long b) { return a * V + b; };
    auto addEdge = [&](long long u, long long v, long long cap) -> bool {
        if (u == v) return false;
        long long key = edgeKey(u, v);
        if (!used.insert(key).second) return false;
        adj[u].push_back({v, cap});
        return true;
    };

    // 1) Guaranteed source -> sink path along the random order.
    long long edgesSoFar = 0;
    for (long long i = 0; i + 1 < V; i++)
        if (addEdge(order[i], order[i + 1], capDist(rng))) edgesSoFar++;

    // 2) Extra random forward edges (earlier -> later in `order`) up to E.
    long long maxAttempts = std::max<long long>(E * 20, 100000);
    long long attempts = 0;
    std::uniform_int_distribution<long long> vDist(0, V - 1);
    while (edgesSoFar < E && attempts < maxAttempts) {
        long long u = vDist(rng), v = vDist(rng);
        if (pos[u] < pos[v] && addEdge(u, v, capDist(rng))) edgesSoFar++;
        attempts++;
    }

    std::ofstream fout(outPath);
    if (!fout) {
        std::cerr << "Cannot open output file: " << outPath << "\n";
        return 1;
    }
    fout << V << " " << edgesSoFar << "\n";
    for (long long u = 0; u < V; u++) {
        fout << u << " " << adj[u].size();
        for (auto &pr : adj[u]) fout << " " << pr.first << " " << pr.second;
        fout << "\n";
    }
    fout << "SOURCE " << source << "\n";
    fout << "SINK " << sink << "\n";

    std::cerr << "Wrote " << outPath << " : V=" << V << " E=" << edgesSoFar
               << " source=" << source << " sink=" << sink << "\n";
    return 0;
}
