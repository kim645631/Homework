#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>


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

class AOE {
private:
    int n;
    vector<vector<pair<int, int>>> adj;
    vector<int> inD;

public:
    AOE(int vertices) : n(vertices), adj(vertices), inD(vertices, 0) {}

    void AddEdge(int u, int v, int weight) {
        if (u < 0 || u >= n || v < 0 || v >= n) return;
        adj[u].push_back(make_pair(v, weight));
        inD[v]++;
    }

    void Time() {
        queue<int> q;
        vector<int> indeg = inD;

        for (int i = 0; i < n; i++) {
            if (indeg[i] == 0) q.push(i);
        }

        vector<int> topo;
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            topo.push_back(u);

            for (const auto& edge : adj[u]) {
                int v = edge.first;
                indeg[v]--;
                if (indeg[v] == 0) q.push(v);
            }
        }

        cout << "\n--- AOE Network: Earliest Time ---" << endl;

        if ((int)topo.size() != n) {
            cout << "錯誤：AOE 網路中包含迴圈（非 DAG），無法計算 ve！" << endl;
            return;
        }

        vector<int> earliest(n, 0);
        for (int u : topo) {
            for (const auto& edge : adj[u]) {
                int v = edge.first;
                int w = edge.second;
                earliest[v] = max(earliest[v], earliest[u] + w);
            }
        }

        int projectFinish = 0;
        for (int i = 0; i < n; i++) {
            cout << "事件 " << i << " 的最早發生時間: " << earliest[i] << endl;
            projectFinish = max(projectFinish, earliest[i]);
        }
        cout << "專案最早完工時間: " << projectFinish << endl;
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
