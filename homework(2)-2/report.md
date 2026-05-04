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
- AOV 網路（Activity On Vertex）：拓撲排序（Topological Sort / Kahn 入度法）
- AOE 網路（Activity On Edge）：事件最早時間（ve）與專案最早完工時間

程式主要分為三個部分：
1. **無權重無向圖（LinkedGraph）**
   使用 adjacency list 圖的結構，提供基本的插入與刪除操作，並實作 DFS、BFS，以及 Connected Components。
2. **加權無向圖（WeightedGraph）**  
   以 adjacency list 儲存（包括鄰點和權重），並實作 Prim 與 Kruskal 求最小生成樹，以及 Dijkstra 求單源最短路徑。
3. **有向網路（AOV / AOE）**
   - **AOV**：以「頂點代表活動、邊代表相依」，使用拓撲排序，輸出執行順序，並能偵測是否存在迴圈。
   - **AOE**：以「邊代表活動（含工期）、頂點代表事件」，本次示範計算事件最早時間 ve，並以終點事件的最大 ve 作為專案最早完工時間輸出。


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
6. **AOV 與 AOE 網路演算法**
   - **AOV（拓撲排序）**：使用 Kahn 入度法，從入度為 0 的節點開始依序輸出；若輸出數量不足 `V`，表示有迴圈無法排序。
   - **AOE（事件最早時間 ve）**：本次僅計算事件最早時間 ve。對每條活動邊 (u→v, w)，用 ve[v] = max(ve[v], ve[u] + w) 更新，最後以終點事件（或所有事件）的最大 ve 作為專案最早完工時間。
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
    bool* vis;

public:
    DFSGraph(int vertices = 0) : Graph(vertices), vis(nullptr) {}
    virtual ~DFSGraph() {}

    virtual void DFS() {
        if (n <= 0) return;
        vis = new bool[n];
        std::fill(vis, vis + n, false);

        cout << "DFS traversal starting from 0: ";
        DFS(0);
        cout << endl;

        delete[] vis;
        vis = nullptr;
    }

    virtual void DFS(const int v) {
        if (v < 0 || v >= n) return;
        vis[v] = true;
        cout << v << " ";
    }
};
class LinkedGraph : public DFSGraph {
private:
    vector<vector<int>> List;

public:
    using DFSGraph::DFS;

    LinkedGraph(int vertices) : DFSGraph(vertices) {
        List.resize(vertices);
    }

    int Degree(int u) const override {
        return u < n ? static_cast<int>(List[u].size()) : 0;
    }

    bool ExistsEdge(int u, int v) const override {
        if (u >= n || v >= n) return false;
        for (int w : List[u]) {
            if (w == v) return true;
        }
        return false;
    }

    void InsertVertex(int v) override {
        List.push_back(vector<int>());
        n++;
    }

    void InsertEdge(int u, int v) override {
        if (u < 0 || u >= n || v < 0 || v >= n) return;
        if (!ExistsEdge(u, v)) {
            List[u].push_back(v);
            List[v].push_back(u);
            e++;
        }
    }

    void DeleteVertex(int v) override {
        if (v < 0 || v >= n) return;

        for (int i = 0; i < n; i++) {
            if (i == v) continue;

            auto& list = List[i];
            auto it = remove(list.begin(), list.end(), v);
            if (it != list.end()) {
                list.erase(it, list.end());
                e--;
            }
        }

        List.erase(List.begin() + v);

        for (int i = 0; i < List.size(); i++) {
            for (int& x : List[i]) {
                if (x > v) x--;
            }
        }

        n--;
    }
    void DeleteEdge(int u, int v) override {
        if (!ExistsEdge(u, v)) return;
        List[u].erase(remove(List[u].begin(), List[u].end(), v), List[u].end());
        List[v].erase(remove(List[v].begin(), List[v].end(), u), List[v].end());
        e--;
    }

    void DFS(const int v) override {
        if (v < 0 || v >= n) return;
        vis[v] = true;
        cout << v << " ";
        for (int w : List[v]) {
            if (!vis[w]) {
                DFS(w);
            }
        }
    }

    void BFS(int v) {
        if (n <= 0) return;
        vis = new bool[n];
        std::fill(vis, vis + n, false);

        cout << "BFS traversal starting from " << v << ": ";
        vis[v] = true;
        cout << v << " ";

        queue<int> q;
        q.push(v);

        while (!q.empty()) {
            int current = q.front();
            q.pop();

            for (int w : List[current]) {
                if (!vis[w]) {
                    q.push(w);
                    vis[w] = true;
                    cout << w << " ";
                }
            }
        }
        cout << endl;
        delete[] vis;
        vis = nullptr;
    }

    void Components() {
        if (n <= 0) return;
        vis = new bool[n];
        std::fill(vis, vis + n, false);

        cout << "--- Connected Components ---" << endl;
        for (int i = 0; i < n; i++) {
            if (!vis[i]) {
                cout << "Component found: ";
                DFS_Component(i);
                cout << endl;
            }
        }
        delete[] vis;
        vis = nullptr;
    }

private:
    void DFS_Component(int v) {
        if (v < 0 || v >= n) return;
        vis[v] = true;
        cout << v << " ";
        for (int w : List[v]) {
            if (!vis[w]) {
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
    vector<vector<pair<int, int>>> List;

public:
    WeightedGraph(int vertices) : n(vertices) {
        List.resize(vertices);
    }

    void InsertEdge(int u, int v, int weight) {
        if (u < n && v < n) {
            List[u].push_back({ v, weight });
            List[v].push_back({ u, weight });
        }
    }

    void Kruskal() {
        vector<Edge> edges;
        for (int u = 0; u < n; u++) {
            for (auto& neighbor : List[u]) {
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
        int min = 0;
        int edgeC = 0;

        for (const auto& edge : edges) {
            int root_u = find(find, edge.u);
            int root_v = find(find, edge.v);

            if (root_u != root_v) {
                cout << "Include edge (" << edge.u << " - " << edge.v << ") with weight " << edge.weight << endl;
                min += edge.weight;
                parent[root_u] = root_v;
                edgeC++;
                if (edgeC == n - 1) break;
            }
        }
        cout << "Total Minimum Cost (Kruskal): " << min << endl;
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

            for (auto& neighbor : List[u]) {
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

            for (auto& neighbor : List[u]) {
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
### AOV and AOE
```cpp
#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <stack>

using namespace std;

class AOV {
private:
    int n;
    vector<vector<int>> List;

public:
    AOV(int vertices) : n(vertices) {
        List.resize(vertices);
    }

    void AddEdge(int u, int v) {
        if (u < n && v < n) {
            List[u].push_back(v);
        }
    }

    void TSort() {
        vector<int> inDegree(n, 0);

        for (int u = 0; u < n; u++) {
            for (int v : List[u]) {
                inDegree[v]++;
            }
        }

        queue<int> q;
        for (int i = 0; i < n; i++) {
            if (inDegree[i] == 0) {
                q.push(i);
            }
        }

        vector<int> topOrder;
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            topOrder.push_back(u);

            for (int v : List[u]) {
                inDegree[v]--;
                if (inDegree[v] == 0) {
                    q.push(v);
                }
            }
        }

        cout << "\n--- AOV Network Topological Sort ---" << endl;
        if (topOrder.size() != n) {
            cout << "錯誤：AOV 網路中包含迴圈，無法進行拓撲排序！" << endl;
        }
        else {
            cout << "執行順序：";
            for (int node : topOrder) {
                cout << node << " ";
            }
            cout << endl;
        }
    }
};

struct AOEEdge {
    int u, v, weight;
};

class AOE {
private:
    int n;
    vector<AOEEdge> edges;

public:
    AOE(int vertices) : n(vertices) {}

    void AddEdge(int u, int v, int weight) {
        edges.push_back({ u, v, weight });
    }

    void Time() {
        vector<int> earliest(n, 0);

        for (const auto& edge : edges) {
            if (earliest[edge.u] + edge.weight > earliest[edge.v]) {
                earliest[edge.v] = earliest[edge.u] + edge.weight;
            }
        }

        cout << "\n--- AOE Network: Earliest Time ---" << endl;
        for (int i = 0; i < n; i++) {
            cout << "事件 " << i << " 的最早發生時間: " << earliest[i] << endl;
        }
    }
};

int main() {
    AOV aov(4);
    aov.AddEdge(0, 1);
    aov.AddEdge(0, 2);
    aov.AddEdge(1, 3);
    aov.AddEdge(2, 3);
    aov.TSort();


    AOE aoe(4);
    aoe.AddEdge(0, 1, 5);
    aoe.AddEdge(0, 2, 3);
    aoe.AddEdge(1, 3, 4);
    aoe.AddEdge(2, 3, 6);
    aoe.Time();

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
|AOV Topological Sort| O(V+E) |
|AOE (ve only) | O(E)|
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
|AOV Topological Sort| O(V+E) |
|AOE (ve only) | O(V+E) |

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
---

### AOV 網路（Topological Sort）
建立 4 個活動（0~3），相依關係：
0→1, 0→2, 1→3, 2→3
```
0 → 1
↓   ↓
2 → 3
```
測試項目：
1. TopologicalSort()

#### 預期輸出
拓撲序不唯一，但需滿足 0 在 1、2 前，且 1、2 在 3 前，例如：
```
Order: 0 1 2 3
```

#### 實際輸出
```
--- AOV Network Topological Sort ---
執行順序：0 1 2 3
```

---

### AOE 網路（事件最早時間 ve）
建立 4 個事件（0~3），活動與工期如下：
0→1(5), 0→2(3), 1→3(4), 2→3(6)
```
 0 -(5)→ 1
 |       |
(3)     (4)
 ↓       ↓
 2 -(6)→ 3
```
測試項目：
1. CalculateEarliestTime()（輸出各事件 ve；最早完工時間可由 max(ve) 判定）
#### 預期結果
```
ve[0]=0, ve[1]=5, ve[2]=3, ve[3]=9  
```
#### 實際輸出
```
--- AOE Network: Earliest Time ---
事件 0 的最早發生時間: 0
事件 1 的最早發生時間: 5
事件 2 的最早發生時間: 3
事件 3 的最早發生時間: 9
```

---

### 測試輸出
```
--- AOV Network Topological Sort ---
執行順序：0 1 2 3

--- AOE Network: Earliest Time ---
事件 0 的最早發生時間: 0
事件 1 的最早發生時間: 5
事件 2 的最早發生時間: 3
事件 3 的最早發生時間: 9
```
### 結論
本次作業完成了圖（Graph）ADT 的設計與實作，並採用 Adjacency List 作為主要資料結構。成功實現無權重無向圖的深度優先搜尋（DFS）、廣度優先搜尋（BFS）與連通元件（Connected Components）判定；並在加權無向圖中完成 Prim、Kruskal 的最小生成樹（MST）以及 Dijkstra 的單源最短路徑演算法。此外，也實作 AOV 與 AOE 網路：AOV 透過拓撲排序輸出合法執行順序並能偵測迴圈；AOE 則示範計算事件最早時間（ve）並得到專案最早完工時間。

從測試結果可驗證：DFS/BFS 能正確遍歷可到達節點、連通元件可正確分組；Prim 與 Kruskal 計算出的 MST 總成本一致可互相驗證；Dijkstra 輸出的最短距離符合預期；AOV 能輸出符合相依關係的拓撲序列；AOE 能得到正確的 ve 與最早完工時間。整體程式具備基本可擴充性，可作為後續加入更多圖論與網路分析演算法的基礎框架。

---

## 申論及開發報告

這次作業採用 adjacency list 的資料結構，兼顧稀疏圖的空間效率與走訪效能；無權重圖部分以 Graph ADT 規範基本操作並完成 DFS、BFS 與連通元件。加權圖部分則以 WeightedGraph 儲存（鄰點、權重），並實作 Prim、Kruskal 的最小生成樹以及 Dijkstra 的單源最短路徑。透過 Prim 與 Kruskal 得到相同的 MST 總成本作為交叉驗證，可確認整體結果具一致性；Dijkstra 的最短距離也符合預期。此外也補充 AOV/AOE 網路：AOV 以拓撲排序輸出合法執行順序；AOE 計算 ve 並判定最早完工時間，作為排程分析基礎。

### 優缺點
#### 優點
- 空間效率高
- 演算法完整性高
- 演算法結果可互相驗證
#### 缺點
- Dijkstra 不支援負權重
- 邊管理仍可優化
