#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
using namespace std;

class Graph {
protected:
    int n; 
    int e; 

public:
    Graph(int vertices = 0) : n(vertices), e(0) {}
    virtual ~Graph() {}

    bool IsEmpty() const { return n == 0; }
    int NumberOfVertices() const { return n; }
    int NumberOfEdges() const { return e; }

    virtual int Degree(int u) const = 0;
    virtual bool ExistsEdge(int u, int v) const = 0;
    virtual void InsertVertex(int v) = 0;
    virtual void InsertEdge(int u, int v) = 0;
    virtual void DeleteVertex(int v) = 0;
    virtual void DeleteEdge(int u, int v) = 0;
};
class DFSGraph : public Graph {
protected:
    bool* visited;

public:
    DFSGraph(int vertices = 0) : Graph(vertices), visited(nullptr) {}
    virtual ~DFSGraph() {}

    virtual void DFS() {
        if (n <= 0) return;
        visited = new bool[n];
        std::fill(visited, visited + n, false);

        cout << "DFS traversal starting from 0: ";
        DFS(0);
        cout << endl;

        delete[] visited;
        visited = nullptr;
    }

    virtual void DFS(const int v) {
        if (v < 0 || v >= n) return;
        visited[v] = true;
        cout << v << " ";
    }
};
class LinkedGraph : public DFSGraph {
private:
    vector<vector<int>> adjList;

public:
    using DFSGraph::DFS;

    LinkedGraph(int vertices) : DFSGraph(vertices) {
        adjList.resize(vertices);
    }

    int Degree(int u) const override {
        return u < n ? static_cast<int>(adjList[u].size()) : 0;
    }

    bool ExistsEdge(int u, int v) const override {
        if (u >= n || v >= n) return false;
        for (int w : adjList[u]) {
            if (w == v) return true;
        }
        return false;
    }

    void InsertVertex(int v) override {
        if (v >= n) {
            adjList.resize(v + 1);
            n = v + 1;
        }
    }

    void InsertEdge(int u, int v) override {
        if (u < 0 || u >= n || v < 0 || v >= n) return;
        adjList[u].push_back(v);
        adjList[v].push_back(u);
        e++;
    }

    void DeleteVertex(int v) override {}
    void DeleteEdge(int u, int v) override {}

    void DFS(const int v) override {
        if (v < 0 || v >= n) return;
        visited[v] = true;
        cout << v << " ";
        for (int w : adjList[v]) {
            if (!visited[w]) {
                DFS(w);
            }
        }
    }

    void BFS(int v) {
        if (n <= 0) return;
        visited = new bool[n];
        std::fill(visited, visited + n, false);

        cout << "BFS traversal starting from " << v << ": ";
        visited[v] = true;
        cout << v << " ";

        queue<int> q;
        q.push(v);

        while (!q.empty()) {
            int current = q.front();
            q.pop();

            for (int w : adjList[current]) {
                if (!visited[w]) {
                    q.push(w);
                    visited[w] = true;
                    cout << w << " ";
                }
            }
        }
        cout << endl;
        delete[] visited;
        visited = nullptr;
    }

    void Components() {
        if (n <= 0) return;
        visited = new bool[n];
        std::fill(visited, visited + n, false);

        cout << "--- Connected Components ---" << endl;
        for (int i = 0; i < n; i++) {
            if (!visited[i]) {
                cout << "Component found: ";
                DFS_Component(i);
                cout << endl;
            }
        }
        delete[] visited;
        visited = nullptr;
    }

private:
    void DFS_Component(int v) {
        if (v < 0 || v >= n) return;
        visited[v] = true;
        cout << v << " ";
        for (int w : adjList[v]) {
            if (!visited[w]) {
                DFS_Component(w);
            }
        }
    }
};

struct Edge {
    int u, v, weight;
    bool operator>(const Edge& other) const {
        return weight > other.weight;
    }
};

class WeightedGraph {
private:
    int n;
    vector<vector<pair<int, int>>> adjList;

public:
    WeightedGraph(int vertices) : n(vertices) {
        adjList.resize(vertices);
    }

    void InsertEdge(int u, int v, int weight) {
        if (u < n && v < n) {
            adjList[u].push_back({v, weight});
            adjList[v].push_back({u, weight});
        }
    }

    void Kruskal() {
        vector<Edge> edges;
        for (int u = 0; u < n; u++) {
            for (auto& neighbor : adjList[u]) {
                int v = neighbor.first;
                int weight = neighbor.second;
                if (u < v) {
                    edges.push_back({u, v, weight});
                }
            }
        }
        sort(edges.begin(), edges.end(), [](const Edge& a, const Edge& b) {
            return a.weight < b.weight;
        });
        vector<int> parent(n);
        for (int i = 0; i < n; i++) parent[i] = i;

        auto find = [&](auto& self, int i) -> int {
            if (parent[i] == i) return i;
            return parent[i] = self(self, parent[i]);
        };

        cout << "\n--- Kruskal's Algorithm ---" << endl;
        int minCost = 0;
        int edgeCount = 0;

        for (const auto& edge : edges) {
            int root_u = find(find, edge.u);
            int root_v = find(find, edge.v);

            if (root_u != root_v) {
                cout << "Include edge (" << edge.u << " - " << edge.v << ") with weight " << edge.weight << endl;
                minCost += edge.weight;
                parent[root_u] = root_v;
                edgeCount++;
                if (edgeCount == n - 1) break;
            }
        }
        cout << "Total Minimum Cost (Kruskal): " << minCost << endl;
    }
    void Prim() {
        vector<bool> inMST(n, false);
        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;

        int startVertex = 0;
        pq.push({0, startVertex});
        int minCost = 0;

        cout << "\n--- Prim's Algorithm ---" << endl;

        while (!pq.empty()) {
            pair<int, int> topElement = pq.top();
            pq.pop();
            int weight = topElement.first;
            int u = topElement.second;

            if (inMST[u]) continue;

            inMST[u] = true;
            minCost += weight;
            cout << "Added vertex " << u << " with edge weight " << weight << endl;

            for (auto& neighbor : adjList[u]) {
                int v = neighbor.first;
                int cost = neighbor.second;
                if (!inMST[v]) {
                    pq.push({cost, v});
                }
            }
        }
        cout << "Total Minimum Cost (Prim): " << minCost << endl;
    }

    void Dijkstra(int start) {
        vector<int> dist(n, 1e9);
        dist[start] = 0;

        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
        pq.push({0, start});

        cout << "\n--- Shortest Paths (Dijkstra) ---" << endl;

        while (!pq.empty()) {
            pair<int, int> topElement = pq.top();
            pq.pop();
            int d = topElement.first;
            int u = topElement.second;

            if (d > dist[u]) continue;

            for (auto& neighbor : adjList[u]) {
                int v = neighbor.first;
                int weight = neighbor.second;

                if (dist[u] + weight < dist[v]) {
                    dist[v] = dist[u] + weight;
                    pq.push({dist[v], v});
                }
            }
        }

        for (int i = 0; i < n; i++) {
            if (dist[i] == 1e9) {
                cout << "Shortest distance from " << start << " to " << i << " is: INF (unreachable)" << endl;
            } else {
                cout << "Shortest distance from " << start << " to " << i << " is: " << dist[i] << endl;
            }
        }
    }
};
int main() {
    cout << "=== Graph Operations ===" << endl;
    LinkedGraph g(5);
    g.InsertEdge(0, 1);
    g.InsertEdge(0, 2);
    g.InsertEdge(1, 3);
    g.InsertEdge(3, 4);
    
    g.DFS();
    g.BFS(0);
    g.Components();
    cout << "\n=== Weighted Graph Algorithms ===" << endl;
    WeightedGraph wg(4);
    wg.InsertEdge(0, 1, 10);
    wg.InsertEdge(0, 2, 6);
    wg.InsertEdge(0, 3, 5);
    wg.InsertEdge(1, 3, 15);
    wg.InsertEdge(2, 3, 4);
    
    wg.Prim();
    wg.Kruskal();
    wg.Dijkstra(0);

    return 0;
}
