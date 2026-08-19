// Random test-file generator for the buddy task (Triangle Counting,
// Betweenness Centrality, Connected Components), which all share the same
// unweighted undirected adjacency-list format (Sections 7.1/8.1/9.1).
//
// Usage:
//   graphgen <V> <E> <outfile>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <random>
#include <set>
#include <string>
#include <vector>

static std::mt19937 rng(std::random_device{}());

static void gen_undirected(int V, int E, const std::string &outfile) {
    std::uniform_int_distribution<int> pick(0, V - 1);
    std::set<std::pair<int, int>> seen; // dedupe, canonical (min,max)
    std::vector<std::vector<int>> adj(V);

    int made = 0;
    int attempts = 0;
    int max_attempts = E * 20 + 1000; // avoid an infinite loop on dense requests
    while (made < E && attempts < max_attempts) {
        attempts++;
        int u = pick(rng), v = pick(rng);
        if (u == v) continue;
        auto key = std::minmax(u, v);
        if (seen.count(key)) continue;
        seen.insert(key);
        adj[u].push_back(v);
        adj[v].push_back(u);
        made++;
    }

    std::ofstream f(outfile);
    f << V << " " << made << "\n";
    for (int u = 0; u < V; u++) {
        f << u << " " << adj[u].size();
        for (int v : adj[u]) f << " " << v;
        f << "\n";
    }
}

int main(int argc, char **argv) {
    if (argc != 4) {
        std::cerr << "Usage: " << argv[0] << " <V> <E> <outfile>\n";
        return 1;
    }
    int V = std::stoi(argv[1]), E = std::stoi(argv[2]);
    std::string out = argv[3];
    gen_undirected(V, E, out);
    return 0;
}
