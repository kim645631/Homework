# 41343112
# 41343121

## 作業二 Graph

## 解題說明
本題目為實作圖（Graph）資料結構及其常見演算法，內容包含：
- 圖的抽象資料型態（ADT）
- 使用鄰接串列（Adjacency List）表示圖
- 圖的走訪：深度優先搜尋（DFS）與廣度優先搜尋（BFS）
- 連通元件（Connected Components）
- 最小生成樹（MST）：Prim、Kruskal
- 單源最短路徑：Dijkstra

程式主要分為兩個部分：
1. **無權重無向圖（LinkedGraph）**
   使用 adjacency list 圖的結構，提供基本的插入與刪除操作，並實作 DFS、BFS，以及 Connected Components。
2. **加權無向圖（WeightedGraph）**  
   以 adjacency list 儲存（包括鄰點和權重），並實作 Prim 與 Kruskal 求最小生成樹，以及 Dijkstra 求單源最短路徑。


### 解題策略
1. **設計 Graph ADT，定義無權重圖的基本操作介面**
   - 以抽象類別 `Graph` 定義頂點數、邊數、查詢、插入與刪除等基本操作，作為無權重圖（LinkedGraph）的共同介面。
   - 加權圖因需處理權重邊與 MST/最短路徑等操作，故以 `WeightedGraph` 類別獨立實作相關演算法。

2. **採用 Adjacency List 表示圖**
   - 相較於 adjacency matrix，鄰接串列在稀疏圖情境下更具效率與記憶體節省特性。
   - 走訪時僅需掃描實際存在的邊，提高執行效率。

3. **DFS 與 BFS 的走訪設計**
   - **DFS**：使用遞迴並搭配 `visited[]`，避免重複拜訪節點。
   - **BFS**：使用 queue 由近到遠分層擴展，依序走訪所有可到達節點。

4. **Connected Components（連通元件）**
   - 遍歷所有節點，找到未訪問的節點並以該節點執行一次 DFS。
   - 每次 DFS 所走訪到的節點集合，即為一個連通元件；重複此流程即可找出所有元件。

5. **加權圖演算法**
   - **Prim（MST）**：每次挑選可擴展生成樹的最小權重邊（或最小代價節點）加入 MST。
   - **Kruskal（MST）**：先將所有邊依權重排序，再使用 Union-Find 判斷是否成環，能加入則加入，直到邊數達到 `V-1`。
   - **Dijkstra（最短路徑）**：每次取出目前距離最短的節點進行 relax 更新；適用於邊權重皆為非負的情況。

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
        adjList.push_back(vector<int>());
        n++;
    }

    void InsertEdge(int u, int v) override {
        if (u < 0 || u >= n || v < 0 || v >= n) return;
        if (!ExistsEdge(u, v)) {
            adjList[u].push_back(v);
            adjList[v].push_back(u);
            e++;
        }
    }

    void DeleteVertex(int v) override {
        if (v < 0 || v >= n) return;

        for (int i = 0; i < n; i++) {
            if (i == v) continue;

            auto& list = adjList[i];
            auto it = remove(list.begin(), list.end(), v);
            if (it != list.end()) {
                list.erase(it, list.end());
                e--;
            }
        }

        adjList.erase(adjList.begin() + v);

        for (int i = 0; i < adjList.size(); i++) {
            for (int& x : adjList[i]) {
                if (x > v) x--;
            }
        }

        n--;
    }
    void DeleteEdge(int u, int v) override {
        if (!ExistsEdge(u, v)) return;
        adjList[u].erase(remove(adjList[u].begin(), adjList[u].end(), v), adjList[u].end());
        adjList[v].erase(remove(adjList[v].begin(), adjList[v].end(), u), adjList[v].end());
        e--;
    }

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
            adjList[u].push_back({ v, weight });
            adjList[v].push_back({ u, weight });
        }
    }

    void Kruskal() {
        vector<Edge> edges;
        for (int u = 0; u < n; u++) {
            for (auto& neighbor : adjList[u]) {
                int v = neighbor.first;
                int weight = neighbor.second;
                if (u < v) {
                    edges.push_back({ u, v, weight });
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
        pq.push({ 0, startVertex });
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
                    pq.push({ cost, v });
                }
            }
        }
        cout << "Total Minimum Cost (Prim): " << minCost << endl;
    }

    void Dijkstra(int start) {
        vector<int> dist(n, 1e9);
        dist[start] = 0;

        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
        pq.push({ 0, start });

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
                    pq.push({ dist[v], v });
                }
            }
        }

        for (int i = 0; i < n; i++) {
            if (dist[i] == 1e9) {
                cout << "Shortest distance from " << start << " to " << i << " is: INF (unreachable)" << endl;
            }
            else {
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
|Prim|O(E log V)|
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
### 無權重無向圖（LinkedGraph）
 建立 5 個頂點（0~4），並加入邊：
(0,1), (0,2), (1,3), (3,4)
```
0 - 1
|   |
2   3 - 4
```
測試項目：
1. DFS()
2. BFS(0)
3. Components()

#### 預期輸出
```
DFS: 0 1 3 4 2
BFS: 0 1 2 3 4
Component: 0 1 3 4 2
```
#### 實際輸出
```
=== Graph Operations ===
DFS traversal starting from 0: 0 1 3 4 2
BFS traversal starting from 0: 0 1 2 3 4
--- Connected Components ---
Component found: 0 1 3 4 2
```
---
### 加權無向圖（WeightedGraph）
 建立 4 個頂點（0~3），並加入加權邊：
(0,1)=10, (0,2)=6, (0,3)=5, (1,3)=15, (2,3)=4
```
 0 -(10)-1
 | \     |
(6) (5) (15)
 |     \ |
 2 -(4)- 3
```
測試項目：
1. Prim()
2. Kruskal()
3. Dijkstra(0)
#### 預期輸出
```
Prim Total Cost: 19
Kruskal Total Cost: 19
0 → 0 = 0
0 → 1 = 10
0 → 2 = 6
0 → 3 = 5
```
#### 實際輸出
```
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
---
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
本次作業完成了圖（Graph）ADT 的設計與實作，並採用Adjacency List作為主要資料結構。成功實現了對無權重圖的深度優先搜尋（DFS）、廣度優先搜尋（BFS）以及連通元件（Connected Components）的判定。同時，也完成了三種針對加權無向圖的演算法，包括 Prim 和 Kruskal 的最小生成樹（MST），以及 Dijkstra 的單源最短路徑演算法。  

從測試結果中可以驗證：DFS 和 BFS 能夠正確遍歷所有可到達節點，連通元件功能也能準確分組。此外，使用 Prim 和 Kruskal 計算出的最小生成樹總成本一致，Dijkstra 亦能輸出起始節點到其他頂點的最短路徑距離。程式具備基本的可擴充性，可作為更多圖演算法的基礎框架。

---

## 申論及開發報告

這次作業採用 adjacency list 的資料結構，兼顧稀疏圖的空間效率與走訪效能；無權重圖部分以 Graph ADT 規範基本操作並完成 DFS、BFS 與連通元件。加權圖部分則以 WeightedGraph 儲存（鄰點、權重），並實作 Prim、Kruskal 的最小生成樹以及 Dijkstra 的單源最短路徑。透過 Prim 與 Kruskal 得到相同的 MST 總成本作為交叉驗證，可確認整體結果具一致性；Dijkstra 的最短距離也符合預期。未來若要提升完整度，可將加權圖的介面也納入更一致的 ADT 設計，並加入最短路徑回溯。

---
### 優缺點
#### 優點
- 空間效率高
- 演算法完整性高
- 演算法結果可互相驗證
#### 缺點
- Dijkstra 不支援負權重
- 邊管理仍可優化
