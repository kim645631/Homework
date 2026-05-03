# 41343112
# 41343121

## 作業二 Graph

## 解題說明
本題主要目標是實作圖(Graph)資料結構與相關演算法，內容包含：
- 圖的抽象資料型態（ADT）
- 圖的表示方式（Adjacency List）
- 圖的走訪（DFS、BFS）
- 連通元件（Connected Components）
- 最小生成樹（Kruskal、Prim）
- 最短路徑（Dijkstra）

本程式分為兩個主要部分：
1. **無權重圖（LinkedGraph）**：使用 adjacency list 表示圖，並實作 DFS、BFS、Connected Components。
2. **加權無向圖（WeightedGraph）**：使用 adjacency list（鄰點, 權重）表示圖，並實作 Prim、Kruskal 取得 MST，以及 Dijkstra 取得單源最短路徑。




### 解題策略
1. **先設計 Graph ADT**
   - 以抽象類別 `Graph` 定義圖的基本操作介面（頂點/邊數、度數、插入邊/點等），讓後續不同表示法可共用同一套介面概念。
2. **使用 Adjacency List 作為圖的表示**
   - 對於稀疏圖（邊數遠小於 V²），Adjacency List 記憶體使用量較佳，走訪鄰居也較直接。
3. **走訪演算法**
   - DFS：遞迴方式實作，並以 `visited[]` 防止重複走訪。
   - BFS：用 queue 進行層級式擴展。
4. **Connected Components**
   - 對每個尚未 visited 的頂點進行一次 DFS，即可得到一個連通元件。
5. **加權圖演算法**
   - Prim：使用 min-priority queue（C++ priority_queue + greater）每次挑最小權重邊擴展 MST。
   - Kruskal：先排序所有邊，再用 Union-Find（Disjoint Set）避免形成 cycle。
   - Dijkstra：用 min-priority queue 每次取出目前距離最短的節點進行 relax（權重需為非負）。


## 程式實作

### IDE:
Microsoft Visual Studio Code C/C++

```cpp
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
```

## 效能分析
(V) = 頂點數、(E) = 邊數
### 時間複雜度
| 名稱                  | 時間複雜度                  |
|-----------------------|-----------------------------|
|DFS| O(V+E)|
|BFS|O(V+E)|
|Connected Components|O(V+E)|
|Prim|O(E log E)（也可寫成 O(E log V)）|
|Kruskal|O(E log E)|
|Dijkstra|O((V+E) log V)|

### 空間複雜度
| 名稱                 | 空間複雜度                  |
|-----------------------|-----------------------------|
|Adjacency List| O(V+E)|
|DFS| O(V)|
|BFS|O(V)|
|Connected Components|O(V)|
|Prim|O(V+E) (pq 最差 O(E))|
|Kruskal|O(V+E)|
|Dijkstra|O(V+E)|

## 測試與驗證

### 測試案例 

| 測試案例 | 輸入參數   | 預期輸出 | 實際輸出 |
|----------|--------------|----------|----------|
| 測試一   |5<br>5 3 1 8 6   | 1 5 3 8 6     |  1 5 3 8 6        |
| 測試二   |6<br>8 5 4 3 2 1   | 1 3 2 8 4 5     |  1 3 2 8 4 5        |
| 測試三   |11<br>5 3 6 1 9 8 2 4 11 7 10   | 1 3 2 4 7 8 6 5 11 9 10     | 1 3 2 4 7 8 6 5 11 9 10       |




### 測試輸出
```
=== Graph Operations ===
DFS traversal starting from 0: 0 1 3 4 2
BFS traversal starting from 0: 0 1 2 3 4
--- Connected Components ---
Component found: 0 1 3 4 2

=== Weighted Graph Algorithms ===

--- Prim's Algorithm ---
Added vertex 0 with edge weight 0
Added vertex 3 with edge weight 5
Added vertex 2 with edge weight 4
Added vertex 1 with edge weight 10
Total Minimum Cost (Prim): 19

--- Kruskal's Algorithm ---
Include edge (2 - 3) with weight 4
Include edge (0 - 3) with weight 5
Include edge (0 - 1) with weight 10
Total Minimum Cost (Kruskal): 19

--- Shortest Paths (Dijkstra) ---
Shortest distance from 0 to 0 is: 0
Shortest distance from 0 to 1 is: 10
Shortest distance from 0 to 2 is: 6
Shortest distance from 0 to 3 is: 5
```
### 結論
本作業以 adjacency list 完成圖的表示，並實作 DFS、BFS 與連通元件判定；在加權圖部分，完成 Prim 與 Kruskal 兩種最小生成樹演算法，並以 Dijkstra 完成單源最短路徑計算。測試結果顯示：
- DFS/BFS 均可正確走訪圖
- Connected Components 可正確辨識連通元件
- Prim 與 Kruskal 的 MST 最小成本一致
- Dijkstra 可得到符合預期的最短距離

---

## 申論及開發報告

### (1) 為何選 adjacency list？
相較於 adjacency matrix（V² 記憶體），adjacency list 更適合本題常見的稀疏圖，記憶體約為 O(V+E)，且 DFS/BFS 對鄰點走訪效率高。

### (2) DFS 與 BFS 的差異
- DFS 適合用於探索、connected components、拓樸性質（本題用於 components）。
- BFS 可得到「層級式」走訪順序，對無權重最短路徑有直接關係（本題僅示範走訪）。

### (3) MST 為何要兩種算法？
- Prim：偏向「從一點擴展」，常用於 adjacency list + priority queue 的情境。
- Kruskal：偏向「從最小邊開始挑」，以 Union-Find 避免 cycle，適合邊集合容易排序的情境。
兩者計算出的 MST 成本應一致，可互相驗證正確性。

### (4) Dijkstra 的限制與注意事項
Dijkstra 需要邊權重非負，否則貪婪選擇不成立。本作業測試邊權重皆為正，因此適用。

### (5) 可改進項目
- Graph ADT 中 `DeleteVertex` / `DeleteEdge` 目前未實作，可補上以符合完整 ADT。
- `InsertEdge` 可增加防重複邊（避免 adjacency list 出現重複鄰點，與 e 計數錯誤）。
- `visited` 可改用 `vector<bool>` 或 `vector<char>` 以避免手動 new/delete，提高安全性。
- Prim 若要輸出「實際選到的邊」，可在 pq 中存 parent 資訊（例如 (cost, v, parent)）。
