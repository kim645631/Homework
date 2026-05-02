# 41343112 41343121


## 作業二 Graph

## 解題說明
- 圖的表示方式（Adjacency Matrix、Adjacency List）
- 圖的走訪（DFS、BFS）
- 連通元件（Connected Components）
- 最小生成樹（Kruskal）
- 最短路徑（Dijkstra）

目標是透過程式實作，理解圖在資料結構中的應用與運作方式。


### 解題策略
### 🔹 Graph Representation
- 使用 adjacency matrix 表示密集圖
- 使用 adjacency list 表示稀疏圖

### 🔹 DFS（Depth First Search）
- 採用遞迴方式
- 利用 visited 陣列避免重複拜訪

### 🔹 BFS（Breadth First Search）
- 使用 queue
- 適合找最短路徑（無權重）

### 🔹 Connected Components
- 對每個未訪問節點執行 DFS
- 計算 DFS 呼叫次數

### 🔹 Kruskal（MST）
- 先排序所有邊（由小到大）
- 使用 Union-Find 避免形成 cycle

### 🔹 Dijkstra（Shortest Path）
- 使用 priority queue
- 每次選擇最短距離節點進行擴展


## 程式實作

### IDE:
Microsoft Visual Studio Code C/C++

```cpp

```

## 效能分析

## 測試與驗證

### 測試案例 

| 測試案例 | 輸入參數   | 預期輸出 | 實際輸出 |
|----------|--------------|----------|----------|
| 測試一   |5<br>5 3 1 8 6   | 1 5 3 8 6     |  1 5 3 8 6        |
| 測試二   |6<br>8 5 4 3 2 1   | 1 3 2 8 4 5     |  1 3 2 8 4 5        |
| 測試三   |11<br>5 3 6 1 9 8 2 4 11 7 10   | 1 3 2 4 7 8 6 5 11 9 10     | 1 3 2 4 7 8 6 5 11 9 10       |



### 測試輸入
```

```
### 測試輸出
```

```
### 結論


## 申論及開發報告
