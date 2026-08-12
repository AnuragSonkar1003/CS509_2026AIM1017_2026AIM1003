#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <chrono>
using namespace std;

const long long INF = 1e18;

bool fileExists(string path) {
    ifstream test(path);
    return test.good();
}

struct UnweightedGraph {
    int V, src;
    vector<int> startPos, nbrList;
};

struct WeightedGraph {
    int V, src;
    vector<int> startPos, nbrList, wtList;
};

UnweightedGraph loadUnweighted(string path) {
    ifstream fin(path);
    int totalV, totalE;
    fin >> totalV >> totalE;

    vector<vector<int>> adj(totalV);
    for (int i = 0; i < totalV; i++) {
        int node, d;
        fin >> node >> d;
        adj[node].resize(d);
        for (int k = 0; k < d; k++) fin >> adj[node][k];
    }
    string tag;
    int src;
    fin >> tag >> src;
    fin.close();

    UnweightedGraph g;
    g.V = totalV;
    g.src = src;
    g.startPos.assign(totalV + 1, 0);
    for (int i = 0; i < totalV; i++) g.startPos[i + 1] = g.startPos[i] + adj[i].size();
    for (int i = 0; i < totalV; i++)
        for (int k = 0; k < adj[i].size(); k++)
            g.nbrList.push_back(adj[i][k]);
    return g;
}

WeightedGraph loadWeighted(string path) {
    ifstream fin(path);
    int totalV, totalE;
    fin >> totalV >> totalE;

    vector<vector<int>> adj(totalV), wt(totalV);
    for (int i = 0; i < totalV; i++) {
        int node, d;
        fin >> node >> d;
        adj[node].resize(d);
        wt[node].resize(d);
        for (int k = 0; k < d; k++) fin >> adj[node][k] >> wt[node][k];
    }
    string tag;
    int src;
    fin >> tag >> src;
    fin.close();

    WeightedGraph g;
    g.V = totalV;
    g.src = src;
    g.startPos.assign(totalV + 1, 0);
    for (int i = 0; i < totalV; i++) g.startPos[i + 1] = g.startPos[i] + adj[i].size();
    for (int i = 0; i < totalV; i++)
        for (int k = 0; k < adj[i].size(); k++) {
            g.nbrList.push_back(adj[i][k]);
            g.wtList.push_back(wt[i][k]);
        }
    return g;
}

// ---------- BFS ----------

void runBFS(string path) {
    UnweightedGraph g = loadUnweighted(path);

    auto startTime = chrono::high_resolution_clock::now();
    vector<int> dist(g.V, -1);
    vector<int> order;
    queue<int> q;
    dist[g.src] = 0;
    q.push(g.src);

    while (!q.empty()) {
        int cur = q.front(); q.pop();
        order.push_back(cur);
        for (int p = g.startPos[cur]; p < g.startPos[cur + 1]; p++) {
            int nxt = g.nbrList[p];
            if (dist[nxt] == -1) {
                dist[nxt] = dist[cur] + 1;
                q.push(nxt);
            }
        }
    }
    auto endTime = chrono::high_resolution_clock::now();
    double timeTaken = chrono::duration<double, milli>(endTime - startTime).count();

    cout << "Algorithm: BFS\n";
    cout << "Source: " << g.src << "\n";
    cout << "Traversal: ";
    for (int v : order) cout << v << " ";
    cout << "\nDistances:\n";
    for (int i = 0; i < g.V; i++) cout << i << " " << dist[i] << "\n";
    cout << "Execution time: " << timeTaken << " ms\n";
}

// ---------- DFS ----------

void dfsVisit(int cur, UnweightedGraph &g, vector<bool> &seen, vector<int> &order) {
    seen[cur] = true;
    order.push_back(cur);
    for (int p = g.startPos[cur]; p < g.startPos[cur + 1]; p++) {
        int nxt = g.nbrList[p];
        if (!seen[nxt]) dfsVisit(nxt, g, seen, order);
    }
}

void runDFS(string path) {
    UnweightedGraph g = loadUnweighted(path);
    vector<bool> seen(g.V, false);
    vector<int> order;

    auto startTime = chrono::high_resolution_clock::now();
    dfsVisit(g.src, g, seen, order);
    auto endTime = chrono::high_resolution_clock::now();
    double timeTaken = chrono::duration<double, milli>(endTime - startTime).count();

    cout << "Algorithm: DFS\n";
    cout << "Source: " << g.src << "\n";
    cout << "Traversal: ";
    for (int v : order) cout << v << " ";
    cout << "\n";
    cout << "Execution time: " << timeTaken << " ms\n";
}

// ---------- SSSP ----------

void runSSSP(string path) {
    WeightedGraph g = loadWeighted(path);

    auto startTime = chrono::high_resolution_clock::now();
    vector<long long> dist(g.V, INF);
    dist[g.src] = 0;
    priority_queue<pair<long long, int>, vector<pair<long long, int>>, greater<>> pq;
    pq.push({0, g.src});

    while (!pq.empty()) {
        auto [d, cur] = pq.top();
        pq.pop();
        if (d > dist[cur]) continue;
        for (int p = g.startPos[cur]; p < g.startPos[cur + 1]; p++) {
            int nxt = g.nbrList[p];
            long long newDist = d + g.wtList[p];
            if (newDist < dist[nxt]) {
                dist[nxt] = newDist;
                pq.push({newDist, nxt});
            }
        }
    }
    auto endTime = chrono::high_resolution_clock::now();
    double timeTaken = chrono::duration<double, milli>(endTime - startTime).count();

    cout << "Algorithm: SSSP\n";
    cout << "Source: " << g.src << "\n";
    cout << "Vertex Distance\n";
    for (int i = 0; i < g.V; i++) {
        if (dist[i] == INF) cout << i << " INF\n";
        else cout << i << " " << dist[i] << "\n";
    }
    cout << "Execution time: " << timeTaken << " ms\n";
}

// ---------- driver ----------

int main(int argc, char *argv[]) {
    if (argc < 3) {
        cerr << "Usage: ./driver_buddy <algorithm> <input_file>\n";
        cerr << "Algorithms: bfs, dfs, sssp\n";
        return 1;
    }

    string algo = argv[1];
    string path = argv[2];

    if (!fileExists(path)) {
        cerr << "Error: input file '" << path << "' not found or cannot be opened.\n";
        return 1;
    }

    if (algo == "bfs") runBFS(path);
    else if (algo == "dfs") runDFS(path);
    else if (algo == "sssp") runSSSP(path);
    else {
        cerr << "Error: unknown algorithm '" << algo << "'.\n";
        cerr << "Choose from: bfs, dfs, sssp\n";
        return 1;
    }

    return 0;
}
