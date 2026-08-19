#include "maxflow.hpp"
#include <algorithm>
#include <chrono>
#include <climits>
#include <queue>

namespace {

struct DinicEdge {
    int to;
    int cap;   // remaining residual capacity
    int rev;   // index of the reverse edge in graph[to]
};

class Dinic {
public:
    explicit Dinic(int n) : n_(n), graph_(n), level_(n), it_(n) {}

    void add_edge(int from, int to, int cap) {
        graph_[from].push_back({to, cap, static_cast<int>(graph_[to].size())});
        graph_[to].push_back({from, 0, static_cast<int>(graph_[from].size()) - 1});
    }

    long long max_flow(int s, int t) {
        long long flow = 0;
        while (bfs(s, t)) {
            std::fill(it_.begin(), it_.end(), 0);
            long long pushed;
            while ((pushed = augment_once(s, t)) > 0) flow += pushed;
        }
        return flow;
    }

    // Reachability from `s` in the FINAL residual graph (edges with
    // remaining capacity > 0), used for min-cut extraction.
    std::vector<char> residual_reachable(int s) const {
        std::vector<char> visited(n_, 0);
        std::queue<int> q;
        visited[s] = 1;
        q.push(s);
        while (!q.empty()) {
            int v = q.front();
            q.pop();
            for (const auto &e : graph_[v]) {
                if (e.cap > 0 && !visited[e.to]) {
                    visited[e.to] = 1;
                    q.push(e.to);
                }
            }
        }
        return visited;
    }

private:
    int n_;
    std::vector<std::vector<DinicEdge>> graph_;
    std::vector<int> level_;
    std::vector<int> it_;

    bool bfs(int s, int t) {
        std::fill(level_.begin(), level_.end(), -1);
        std::queue<int> q;
        level_[s] = 0;
        q.push(s);
        while (!q.empty()) {
            int v = q.front();
            q.pop();
            for (const auto &e : graph_[v]) {
                if (e.cap > 0 && level_[e.to] < 0) {
                    level_[e.to] = level_[v] + 1;
                    q.push(e.to);
                }
            }
        }
        return level_[t] >= 0;
    }

    // Finds and pushes one augmenting path through the level graph, using
    // an EXPLICIT stack (not recursion) so it stays safe at large V.
    // Returns the bottleneck pushed, or 0 if no path remains this phase.
    long long augment_once(int s, int t) {
        std::vector<int> path_v = {s};
        std::vector<int> path_e;

        while (!path_v.empty()) {
            int v = path_v.back();
            if (v == t) {
                long long bottleneck = LLONG_MAX;
                for (size_t i = 0; i + 1 < path_v.size(); i++)
                    bottleneck = std::min<long long>(bottleneck, graph_[path_v[i]][path_e[i]].cap);
                for (size_t i = 0; i + 1 < path_v.size(); i++) {
                    int u = path_v[i], eidx = path_e[i];
                    graph_[u][eidx].cap -= static_cast<int>(bottleneck);
                    int rev = graph_[u][eidx].rev;
                    int to = graph_[u][eidx].to;
                    graph_[to][rev].cap += static_cast<int>(bottleneck);
                }
                return bottleneck;
            }

            bool advanced = false;
            while (it_[v] < static_cast<int>(graph_[v].size())) {
                const auto &e = graph_[v][it_[v]];
                if (e.cap > 0 && level_[v] < level_[e.to]) {
                    path_v.push_back(e.to);
                    path_e.push_back(it_[v]);
                    advanced = true;
                    break;   // leave it_[v] here; resume from it if this branch dead-ends
                }
                it_[v]++;
            }
            if (!advanced) {
                level_[v] = -1;   // dead end: never revisit in this phase
                path_v.pop_back();
                if (!path_e.empty()) path_e.pop_back();
            }
        }
        return 0;
    }
};

}  // namespace

MaxflowResult maxflow_mincut(const CSRGraph &g, int source, int sink) {
    MaxflowResult result;
    auto t0 = std::chrono::high_resolution_clock::now();

    // Build the residual network from the already-prepared CSR.
    // Per the spec, this initialization is part of the timed algorithm call.
    Dinic dinic(g.V);
    for (int u = 0; u < g.V; u++)
        for (int k = g.row_ptr[u]; k < g.row_ptr[u + 1]; k++)
            dinic.add_edge(u, g.col_idx[k], g.weight[k]);

    result.max_flow_value = dinic.max_flow(source, sink);

    // Min-cut extraction from the final residual graph (also timed).
    std::vector<char> reachable = dinic.residual_reachable(source);
    for (int v = 0; v < g.V; v++)
        (reachable[v] ? result.source_side : result.sink_side).push_back(v);

    long long cut_cap = 0;
    for (int u = 0; u < g.V; u++) {
        if (!reachable[u]) continue;
        for (int k = g.row_ptr[u]; k < g.row_ptr[u + 1]; k++) {
            int v = g.col_idx[k];
            if (!reachable[v]) {
                result.cut_edges.push_back({u, v, g.weight[k]});
                cut_cap += g.weight[k];
            }
        }
    }
    result.min_cut_capacity = cut_cap;

    auto t1 = std::chrono::high_resolution_clock::now();
    result.time_ms = std::chrono::duration<double, std::milli>(t1 - t0).count();
    return result;
}
