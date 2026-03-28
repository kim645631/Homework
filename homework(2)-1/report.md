# 41343121

## 作業一 Max/Min Heap

## 解題說明
將課堂所學的MaxHeap實作MinHeap並以MinPQ的架構去編寫
### 解題策略
目標:把輸入的 n 個整數，依照「由小到大」排序並輸出
1. 先定義 ADT：MinPQ<T>（抽象類別）
   
   - IsEmpty()：判斷是否為空
   - Top()：回傳目前最小值
   - Push(x)：插入元素
   - Pop()：刪除最小值
  
2. 實作 MinPQ<T>：class MinHeap : public MinPQ<T>
資料結構選擇：


   - 用陣列 heap 存完全二元樹

   - 採 1-based indexing：根節點在 heap[1]

      - parent = i/2
      - left child = 2*i
      - right child = 2*i + 1
## 程式實作

### IDE:
Microsoft Visual Studio Code C/C++

```cpp
#include <iostream>
#include <string>
using namespace std;

template <class T>
class MinPQ {
public:
    virtual ~MinPQ() {}
    virtual bool IsEmpty() const = 0;
    virtual const T& Top() const = 0;
    virtual void Push(const T& x) = 0;
    virtual void Pop() = 0;
};

template <class T>
class MinHeap : public MinPQ<T> {
private:
    T* heap;        // 1-based: heap[1] is min
    int heapSize;
    int capacity;

    void ChangeSize1D(int newCapacity) {
        if (newCapacity < 1) newCapacity = 1;

        T* newHeap = new T[newCapacity + 1];
        int copySize = (heapSize < newCapacity) ? heapSize : newCapacity;

        for (int i = 1; i <= copySize; i++) newHeap[i] = heap[i];

        delete[] heap;
        heap = newHeap;
        capacity = newCapacity;

        if (heapSize > capacity) heapSize = capacity;
    }

public:
    explicit MinHeap(int theCapacity = 10)
        : heap(nullptr), heapSize(0), capacity(theCapacity) {
        if (capacity < 1) capacity = 1;
        heap = new T[capacity + 1];
    }

    ~MinHeap() override { delete[] heap; }

    bool IsEmpty() const override { return heapSize == 0; }

    const T& Top() const override {
        if (IsEmpty()) throw string("MinHeap::Top: heap is empty");
        return heap[1];
    }

    void Push(const T& x) override {
        if (heapSize == capacity) ChangeSize1D(capacity * 2);

        int currentNode = ++heapSize;
        while (currentNode != 1 && x < heap[currentNode / 2]) {
            heap[currentNode] = heap[currentNode / 2];
            currentNode /= 2;
        }
        heap[currentNode] = x;
    }

    void Pop() override {
        if (IsEmpty()) throw string("MinHeap::Pop: heap is empty");

        T lastE = heap[heapSize--];

        int currentNode = 1;
        int child = 2;

        while (child <= heapSize) {
            if (child < heapSize && heap[child] > heap[child + 1]) child++; // smaller child
            if (lastE <= heap[child]) break;

            heap[currentNode] = heap[child];
            currentNode = child;
            child *= 2;
        }
        heap[currentNode] = lastE;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    MinHeap<int> h((n > 10) ? n : 10);

    for (int i = 0; i < n; i++) {
        int x;
        cin >> x;
        h.Push(x);
    }

    while (!h.IsEmpty()) {
        cout << h.Top();
        h.Pop();
        if (!h.IsEmpty()) cout << " ";
    }
    cout << "\n";
    return 0;
}
```

## 效能分析
1. 空間複雜度：
- O(n)
2. 時間複雜度：
- IsEmpty()：O(1)
- Top()：O(1)
- Push(x)：O(log n)
- Pop()：O(log n)
## 測試與驗證

### 測試案例 

| 測試案例 | 輸入參數  | 預期輸出 | 實際輸出 |
|----------|--------------|----------|----------|
| 測試一   |5   | 3        | 3        |


### 編譯與執行指令

```shell

```
### 結論
本次作業以Min-Heap實作最小優先佇列 MinPQ。利用堆的性質可讓最小值永遠位於根節點heap[1]，使用:

- IsEmpty：判斷 heap 是否為空

- Top（取最小值）：回傳最小元素（root）

- Push（插入)：若已滿使用擴容,插入到最後並往上調整

- Pop（刪除最小值）：把最後一個元素拿出,放到 root往下調整


## 申論及開發報告
本題目要求先撰寫一個最小優先佇列的抽象類別 MinPQ<T>，其功能包含判斷是否為空、取得最小元素、插入元素、刪除最小元素等基本操作；再以最小堆（Min-Heap）實作具體類別 MinHeap<T>，並完成所有虛擬函式。

使用了跟MaxHeap 一致的程式架構，本實作採用陣列表示完全二元樹
最小堆需滿足堆序性質：任一節點的值皆小於等於其子節點，因此根節點即為全堆最小值。插入與刪除操作會破壞堆序，必須透過「Push上濾」與「Pop下濾」恢復堆的結構與堆序，並確保各操作複雜度與 MaxHeap 對應一致。
### 優點

- 時間效率好、符合題目複雜度
- 資料結構簡單、實作直觀
- 可動態擴充容量

### 缺點
- 記憶體管理較容易出錯
- 擴充容量的代價（單次可能 O(n)）
   -擴充時會配置新陣列並複製資料，單次成本是 O(n)。不過平均而言，插入仍可視為接近 O(log n)擴充時會有額外複製成本。

--------------------------------------------------------------------------------------------------------------------------------------------------------------------

## 作業一 Binary Search Tree

## 解題說明

### 解題策略
## 程式實作

### IDE:
Microsoft Visual Studio Code C/C++

```cpp

```

## 效能分析
1. 空間複雜度：
2. 時間複雜度：

## 測試與驗證

### 測試案例

| 測資 | 輸入參數 $n$ , 元素 | 預期輸出 | 實際輸出 |
|----------|--------------|----------|----------|
| 測試一   | $n = 1$ , a | () (a)       | () (a)       |
| 測試二   | $m = 2$ , a b | () (b) (a) (a,b)  | () (b) (a) (a,b)       |
| 測試三   | $m = 3$ , a b c | () (c) (b) (b,c) (a) (a,c) (a,b) (a,b,c) | () (c) (b) (b,c) (a) (a,c) (a,b) (a,b,c) |
### 編譯與執行指令

```shell
$ g++ pow.cpp -std=c++17 -o pow.exe
$ .\pow.exe

```

### 結論


## 申論及開發報告

