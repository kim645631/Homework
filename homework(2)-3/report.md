# 41343112
# 41343121

## 作業三 Sorting Project

## 解題說明
本題目主要目標為實作多種排序演算法，並比較其在不同輸入資料與不同資料量下的執行效能。程式中共實作五種排序方式：

1. Insertion Sort  
2. Quick Sort（Median-of-Three）  
3. Iterative Merge Sort  
4. Heap Sort  
5. Composite Sort  

其中 Composite Sort 會依資料量大小自動選擇適合的排序法，以提升整體效能。

程式同時包含：

- Worst-case 測試
- Average-case 測試
- Correctness Test
- 計時系統
- 結果輸出
### 解題策略
### 1. Insertion Sort

Insertion Sort 採用逐步插入方式進行排序。

策略如下：

- 從第二個元素開始
- 將元素插入左側已排序區域
- 若左側元素較大則向右移動

此方法在小型資料下效率良好，因此 Composite Sort 中也將其用於小資料量排序。

---

### 2. Quick Sort

Quick Sort 採用 Divide and Conquer（分治法）概念：

1. 選擇 pivot
2. 進行 partition
3. 遞迴排序左右子區間

本程式使用：

```cpp
median-of-three
```

作為 pivot 選擇方式。

選擇：

- left
- middle
- right

三者中的中位數，可降低：

- 已排序資料
- 反向資料

造成 Worst-case 的機率。

此外，當子區間長度小於 16 時：

```cpp
cut = 16
```

改用 Insertion Sort 提升效率。

---

### 3. Iterative Merge Sort

本專題使用 Iterative Merge Sort，而非 Recursive Merge Sort。

其策略為：

- 先合併長度為 1 的區塊
- 再依序合併：
  - 2
  - 4
  - 8
  - 16

直到整個陣列完成排序。

此方法可避免遞迴造成的額外函式呼叫成本。

---

### 4. Heap Sort

Heap Sort 使用：

- Max Heap

進行排序。

流程如下：

1. 建立 Max Heap
2. 將最大值移至尾端
3. 縮小 Heap 範圍
4. 重新下沉調整

重複直到排序完成。

---

### 5. Composite Sort

Composite Sort 根據資料量大小選擇排序方式：

```cpp
if (n <= 32)
    ins(a);
else
    mrg(a);
```

小型資料使用：

- Insertion Sort

大型資料使用：

- Merge Sort

藉此提升整體排序效率。
## 程式實作

### IDE:
Microsoft Visual Studio Code C/C++

```cpp
#include <algorithm>  
#include <chrono>     
#include <iomanip>    
#include <iostream>   
#include <numeric>   
#include <random>     
#include <string>     
#include <vector>    
using namespace std; 

using Clk = chrono::high_resolution_clock;   
using Us = chrono::duration<double, micro>; 

enum ST { INS, QCK, MRG, HEP, CMP };  

struct R {          
    int n;          
    string alg;     
    string kind;    
    double us;      
    int times;      
};

mt19937 gen(20260524);  

string nm(ST t) {                         
    if (t == INS) return "Insertion Sort"; 
    if (t == QCK) return "Quick Sort";     
    if (t == MRG) return "Merge Sort";     
    if (t == HEP) return "Heap Sort";      
    if (t == CMP) return "Composite Sort"; 
    return "Unknown";                      
}

vector<int> seq(int n) {          
    vector<int> a(n);             
    iota(a.begin(), a.end(), 1);  
    return a;                     
}

vector<int> rev(int n) {          
    vector<int> a(n);             
    for (int i = 0; i < n; i++) { 
        a[i] = n - i;             
    }
    return a;                     
}

vector<int> rnd(int n) {                 
    vector<int> a = seq(n);              
    shuffle(a.begin(), a.end(), gen);    
    return a;                            
}

void ins(vector<int>& a) {                    
    for (int i = 1; i < (int)a.size(); i++) { 
        int key = a[i];                       
        int j = i - 1;                        
        while (j >= 0 && a[j] > key) {        
            a[j + 1] = a[j];                  
            j--;                             
        }
        a[j + 1] = key;                       
    }
}

int med3(vector<int>& a, int l, int r) { 
    int m = l + (r - l) / 2;             
    if (a[m] < a[l]) swap(a[m], a[l]);   
    if (a[r] < a[l]) swap(a[r], a[l]);   
    if (a[r] < a[m]) swap(a[r], a[m]);   
    swap(a[m], a[r - 1]);                
    return a[r - 1];                     
}

void qrec(vector<int>& a, int l, int r) { 
    int cut = 16;                         
    if (l + cut <= r) {                   
        int p = med3(a, l, r);            
        int i = l;                        
        int j = r - 1;                    
        while (true) {                    
            while (a[++i] < p) {          
            }
            while (p < a[--j]) {          
            }
            if (i < j) {                  
                swap(a[i], a[j]);         
            }
            else {                      
                break;                    
            }
        }
        swap(a[i], a[r - 1]);             
        qrec(a, l, i - 1);                
        qrec(a, i + 1, r);                
    }
    else {                              
        for (int x = l + 1; x <= r; x++) {
            int key = a[x];               
            int y = x - 1;                
            while (y >= l && a[y] > key) {
                a[y + 1] = a[y];          
                y--;                      
            }
            a[y + 1] = key;               
        }
    }
}

void qck(vector<int>& a) {             
    if (a.size() > 1) {                
        qrec(a, 0, (int)a.size() - 1); 
    }
}

void mrg(vector<int>& a) {                 
    int n = (int)a.size();                 
    vector<int> tmp(n);                    
    for (int w = 1; w < n; w *= 2) {      
        for (int l = 0; l < n; l += 2 * w) {
            int m = min(l + w, n);         
            int r = min(l + 2 * w, n);     
            int i = l;                     
            int j = m;                     
            int k = l;                     
            while (i < m && j < r) {       
                if (a[i] <= a[j]) {        
                    tmp[k] = a[i];         
                    i++;                   
                }
                else {                   
                    tmp[k] = a[j];         
                    j++;                   
                }
                k++;                       
            }
            while (i < m) {                
                tmp[k++] = a[i++];         
            }
            while (j < r) {                
                tmp[k++] = a[j++];         
            }
            for (int x = l; x < r; x++) {  
                a[x] = tmp[x];             
            }
        }
    }
}

void down(vector<int>& a, int s, int e) { 
    int root = s;                         
    while (root * 2 + 1 <= e) {           
        int lc = root * 2 + 1;            
        int rc = lc + 1;                  
        int big = root;                   
        if (a[big] < a[lc]) {             
            big = lc;                    
        }
        if (rc <= e && a[big] < a[rc]) {  
            big = rc;                     
        }
        if (big == root) {                
            return;                       
        }
        swap(a[root], a[big]);            
        root = big;                       
    }
}

void heap(vector<int>& a) {                    
    int n = (int)a.size();                     
    for (int s = n / 2 - 1; s >= 0; s--) {     
        down(a, s, n - 1);                     
    }
    for (int e = n - 1; e > 0; e--) {          
        swap(a[0], a[e]);                      
        down(a, 0, e - 1);                     
    }
}

void cmp(vector<int>& a) { 
    int n = (int)a.size(); 
    if (n <= 32) {         
        ins(a);            
    }
    else {               
        mrg(a);            
    }
}

void run(ST t, vector<int>& a) {  
    if (t == INS) ins(a);         
    else if (t == QCK) qck(a);    
    else if (t == MRG) mrg(a);    
    else if (t == HEP) heap(a);   
    else if (t == CMP) cmp(a);    
}

bool ok(ST t) {                         
    vector<vector<int>> tests = {       
        {},                             
        {1},                            
        {2, 1},                         
        {5, 1, 3, 3, 2, 9, 0},          
        rev(100),                       
        rnd(257)                        
    };
    for (vector<int> a : tests) {       
        vector<int> ans = a;            
        sort(ans.begin(), ans.end());   
        run(t, a);                      
        if (a != ans) {                 
            return false;               
        }
    }
    return true;                       
}

double rpt(ST t, const vector<int>& src, int& times) { 
    times = 1;                                        
    double total = 0.0;                                
    while (times <= 1048576) {                         
        auto st = Clk::now();                         
        for (int i = 0; i < times; i++) {             
            vector<int> a = src;                      
            run(t, a);                                 
        }
        auto ed = Clk::now();                          
        total = Us(ed - st).count();                   
        if (total >= 100000.0) {                       
            break;                                     
        }
        times *= 2;                                   
    }
    return total / times;                              
}

vector<int> mwRec(const vector<int>& s) {      
    int n = (int)s.size();                     
    if (n <= 1) {                              
        return s;                             
    }
    vector<int> l;                            
    vector<int> r;                             
    for (int i = 0; i < n; i++) {             
        if (i % 2 == 0) {                    
            l.push_back(s[i]);                
        }
        else {                               
            r.push_back(s[i]);                 
        }
    }
    l = mwRec(l);                              
    r = mwRec(r);                             
    l.insert(l.end(), r.begin(), r.end());     
    return l;                                
}

vector<int> wdata(ST t, int n) { 
    if (t == INS) {              
        return rev(n);           
    }
    if (t == MRG) {              
        return mwRec(seq(n));    
    }
    return rnd(n);               
}

R worst(ST t, int n, int rt) {              
    if (t == QCK || t == HEP || t == CMP) { 
        double mx = 0.0;                    
        for (int i = 0; i < rt; i++) {      
            int times = 1;                  
            vector<int> a = rnd(n);         
            double now = rpt(t, a, times);  
            mx = max(mx, now);              
        }
        return { n, nm(t), "worst", mx, rt }; 
    }
    int times = 1;                          
    vector<int> a = wdata(t, n);           
    double now = rpt(t, a, times);         
    return { n, nm(t), "worst", now, times }; 
}

R avg(ST t, int n, int rt) {                  
    vector<vector<int>> samples;              
    for (int i = 0; i < rt; i++) {           
        samples.push_back(rnd(n));            
    }
    auto st = Clk::now();                     
    for (vector<int> a : samples) {          
        run(t, a);                            
    }
    auto ed = Clk::now();                    
    double mean = Us(ed - st).count() / rt;  
    return { n, nm(t), "average", mean, rt };   
}

void show(const vector<R>& rs) {                   
    cout << fixed << setprecision(3);              
    cout << "\nTiming summary, microseconds per sort\n"; 
    cout << "n,algorithm,data_kind,microseconds,trials\n"; 
    for (R x : rs) {                               
        cout << x.n << ","                         
            << x.alg << ","                       
            << x.kind << ","                      
            << x.us << ","                       
            << x.times << "\n";                  
    }
}

int main() {                                                   
    vector<ST> algs = { INS, QCK, MRG, HEP, CMP };              
    vector<int> ns = { 500, 1000, 2000, 3000, 4000, 5000 };      
    vector<R> rs;                                              
    cout << "Timer: chrono::high_resolution_clock\n";          
    cout << "Clock accuracy estimate: "                       
        << (double)Clk::period::num / Clk::period::den * 1000000.0 
        << " microseconds per tick\n";                        
    for (ST t : algs) {                                        
        if (!ok(t)) {                                          
            cout << nm(t) << " failed correctness test.\n";    
            return 1;                                          
        }
    }
    cout << "Correctness tests passed.\n";                    
    for (int n : ns) {                                       
        int rt = max(10, 10000 / n);                           
        for (ST t : algs) {                                    
            rs.push_back(worst(t, n, rt));                     
            rs.push_back(avg(t, n, rt));                       
        }
    }
    show(rs);                                                  
    cout << "\nFinished. Copy these results into Excel or your report if needed.\n"; 
    return 0;                                                  
}
```
## 效能分析


### 時間複雜度
| 排序法 | Best Case | Average Case | Worst Case |
|---|---|---|---|
| Insertion Sort | O(n) | O(n²) | O(n²) |
| Quick Sort | O(n log n) | O(n log n) | O(n²) |
| Merge Sort | O(n log n) | O(n log n) | O(n log n) |
| Heap Sort | O(n log n) | O(n log n) | O(n log n) |
| Composite Sort | O(n) / O(n log n) | O(n log n) | O(n log n) |
### Insertion Sort

Worst-case 發生於反向排列：

```text
n, n-1, ..., 1
```

其時間複雜度為：

```text
O(n²)
```

---

### Quick Sort

平均情況下：

```text
O(n log n)
```

但若 pivot 選擇不佳，可能退化為：

```text
O(n²)
```

因此本程式使用 Median-of-Three 改善效能。

---

### Merge Sort

Merge Sort 不受資料排列影響，其時間複雜度固定為：

```text
O(n log n)
```

---

### Heap Sort

Heap Sort 同樣維持：

```text
O(n log n)
```

且對資料排列較不敏感。

---

### 空間複雜度

| 排序法 | 空間複雜度 |
|---|---|
| Insertion Sort | O(1) |
| Quick Sort | O(log n) |
| Merge Sort | O(n) |
| Heap Sort | O(1) |
| Composite Sort | O(n) |

---

### Merge Sort

需額外使用：

```cpp
vector<int> tmp
```

作為合併暫存空間，因此需要 O(n) 額外記憶體。

---

### Heap Sort

Heap Sort 為 In-place Sort，因此額外空間需求較低。


本程式首先進行 Correctness Test，測試內容包含：

- 空陣列
- 單一元素
- 反向排列
- 重複元素
- 隨機排列

並使用：

```cpp
sort(ans.begin(), ans.end());
```

作為標準答案比較。

若排序結果與標準答案不同，則視為排序失敗。

---

### Worst-case 測試

#### Insertion Sort

使用：

```text
n, n-1, ..., 1
```

作為 Worst-case。

---

#### Merge Sort

使用：

```cpp
mwRec()
```

遞迴產生 Merge Sort Worst-case 資料。

此方法會使 merge 過程中比較次數增加。

---

#### Quick Sort 與 Heap Sort

由於難以直接構造真正 Worst-case，因此程式使用：

```cpp
random permutation
```

並測試多組資料：

```cpp
rt = max(10, 10000 / n)
```

取其中耗時最大者作為近似 Worst-case。

---

### Average-case 測試

Average-case 使用多組隨機排列進行測試，並計算平均執行時間。

---

### 計時方法

本程式使用：

```cpp
chrono::high_resolution_clock
```

進行高精度時間量測。

單位為：

```text
microseconds
```

為避免量測誤差，程式會自動增加重複執行次數：

```cpp
times *= 2;
```

直到總執行時間超過：

```text
100000 microseconds
```

以提高量測穩定性。

---

## 結論

本專題成功實作五種排序演算法，並完成：

- Correctness Testing
- Worst-case Analysis
- Average-case Analysis
- Performance Measurement

實驗結果顯示：

- Insertion Sort 適合小型資料
- Quick Sort 平均效能最佳
- Merge Sort 效能最穩定
- Heap Sort 不易受到輸入資料影響

此外，Composite Sort 可根據資料量選擇不同排序法，在小型資料與大型資料間取得更好的平衡。

---

## 申論及開發報告

本專題除了實作排序演算法外，也深入理解：

1. Divide and Conquer  
2. Heap Data Structure  
3. 時間複雜度分析  
4. Worst-case Generation  
5. 高精度計時方法  

在實作過程中，較具挑戰性的部分包括：

- Median-of-Three pivot 選擇
- Iterative Merge Sort 設計
- Merge Sort Worst-case 生成
- 計時誤差控制

此外，也發現理論複雜度與實際執行結果仍可能受到：

- Compiler Optimization
- CPU Cache
- 記憶體存取方式
- 資料排列方式

等因素影響。

透過本專題，更深入理解：

- 各排序法的適用情境
- 理論與實際效能差異
- 混合式排序策略的重要性
