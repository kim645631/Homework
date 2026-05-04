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
