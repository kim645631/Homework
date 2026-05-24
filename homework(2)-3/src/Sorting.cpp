#include <algorithm>  // 使用 sort、shuffle、max、min、swap 等工具
#include <chrono>     // 使用 high_resolution_clock 進行計時
#include <iomanip>    // 使用 fixed、setprecision 控制輸出格式
#include <iostream>   // 使用 cout 輸出到畫面
#include <numeric>    // 使用 iota 產生連續數字
#include <random>     // 使用 mt19937 產生隨機排列
#include <string>     // 使用 string 儲存文字
#include <vector>     // 使用 vector 儲存資料

using namespace std;  // 省略 std::，讓程式比較簡短

using Clk = chrono::high_resolution_clock;   // 將計時器型別取名為 Clk
using Us = chrono::duration<double, micro>;  // 將時間差轉成微秒

enum ST { INS, QCK, MRG, HEP, CMP };  // 定義排序種類

struct R {          // 儲存一筆實驗結果
    int n;          // 資料筆數
    string alg;     // 排序法名稱
    string kind;    // 測試種類：worst 或 average
    double us;      // 執行時間，單位為微秒
    int times;      // 測試次數或重複次數
};

mt19937 gen(20260524);  // 固定亂數種子，讓每次產生的隨機資料可重現

string nm(ST t) {                         // 將排序代號轉成文字名稱
    if (t == INS) return "Insertion Sort"; // 插入排序
    if (t == QCK) return "Quick Sort";     // 快速排序
    if (t == MRG) return "Merge Sort";     // 合併排序
    if (t == HEP) return "Heap Sort";      // 堆積排序
    if (t == CMP) return "Composite Sort"; // 組合排序
    return "Unknown";                      // 預防未知排序種類
}

vector<int> seq(int n) {          // 產生 1 到 n 的正向資料
    vector<int> a(n);             // 建立大小為 n 的 vector
    iota(a.begin(), a.end(), 1);  // 從 1 開始依序填入
    return a;                     // 回傳產生好的資料
}

vector<int> rev(int n) {          // 產生反向資料
    vector<int> a(n);             // 建立大小為 n 的 vector
    for (int i = 0; i < n; i++) { // 從第 0 格到第 n-1 格
        a[i] = n - i;             // 填入 n, n-1, ..., 1
    }
    return a;                     // 回傳反向資料
}

vector<int> rnd(int n) {                 // 產生隨機排列資料
    vector<int> a = seq(n);              // 先產生 1 到 n
    shuffle(a.begin(), a.end(), gen);    // 將資料順序打亂
    return a;                            // 回傳隨機排列
}

void ins(vector<int>& a) {                    // Insertion Sort 插入排序
    for (int i = 1; i < (int)a.size(); i++) { // 從第二個元素開始
        int key = a[i];                       // 暫存目前要插入的值
        int j = i - 1;                        // j 指向左邊已排序區段
        while (j >= 0 && a[j] > key) {        // 左邊元素比 key 大時
            a[j + 1] = a[j];                  // 將左邊元素往右移
            j--;                              // 繼續往左比較
        }
        a[j + 1] = key;                       // 將 key 放到正確位置
    }
}

int med3(vector<int>& a, int l, int r) { // Quick Sort 的三數取中
    int m = l + (r - l) / 2;             // 找出中間位置
    if (a[m] < a[l]) swap(a[m], a[l]);   // 保證 left 不大於 mid
    if (a[r] < a[l]) swap(a[r], a[l]);   // 保證 left 不大於 right
    if (a[r] < a[m]) swap(a[r], a[m]);   // 保證 mid 不大於 right
    swap(a[m], a[r - 1]);                // 將 pivot 暫時放到 r-1
    return a[r - 1];                     // 回傳 pivot
}

void qrec(vector<int>& a, int l, int r) { // Quick Sort 遞迴函式
    int cut = 16;                         // 小區間門檻值
    if (l + cut <= r) {                   // 區間夠大時使用 Quick Sort
        int p = med3(a, l, r);            // 選擇 pivot
        int i = l;                        // 左指標
        int j = r - 1;                    // 右指標
        while (true) {                    // 開始 partition
            while (a[++i] < p) {          // 從左找不小於 pivot 的元素
            }
            while (p < a[--j]) {          // 從右找不大於 pivot 的元素
            }
            if (i < j) {                  // 如果兩指標尚未交錯
                swap(a[i], a[j]);         // 交換左右兩邊放錯的元素
            }
            else {                      // 如果指標交錯
                break;                    // partition 完成
            }
        }
        swap(a[i], a[r - 1]);             // 將 pivot 放回正確位置
        qrec(a, l, i - 1);                // 遞迴排序左半邊
        qrec(a, i + 1, r);                // 遞迴排序右半邊
    }
    else {                              // 區間太小時
        for (int x = l + 1; x <= r; x++) {// 使用插入排序處理小區間
            int key = a[x];               // 暫存目前元素
            int y = x - 1;                // 從左邊開始比較
            while (y >= l && a[y] > key) {// 左邊元素較大時
                a[y + 1] = a[y];          // 往右移動
                y--;                      // 繼續往左
            }
            a[y + 1] = key;               // 放到正確位置
        }
    }
}

void qck(vector<int>& a) {             // Quick Sort 入口
    if (a.size() > 1) {                // 資料超過一筆才需要排序
        qrec(a, 0, (int)a.size() - 1); // 排序整個陣列
    }
}

void mrg(vector<int>& a) {                 // Iterative Merge Sort
    int n = (int)a.size();                 // 取得資料大小
    vector<int> tmp(n);                    // 暫存合併結果
    for (int w = 1; w < n; w *= 2) {       // w 是目前合併區段長度
        for (int l = 0; l < n; l += 2 * w) {// 每次合併兩個相鄰區段
            int m = min(l + w, n);         // 中間位置
            int r = min(l + 2 * w, n);     // 右邊界
            int i = l;                     // 左半段指標
            int j = m;                     // 右半段指標
            int k = l;                     // tmp 指標
            while (i < m && j < r) {       // 左右兩段都還有資料
                if (a[i] <= a[j]) {        // 左邊元素較小
                    tmp[k] = a[i];         // 放入左邊元素
                    i++;                   // 左指標右移
                }
                else {                   // 右邊元素較小
                    tmp[k] = a[j];         // 放入右邊元素
                    j++;                   // 右指標右移
                }
                k++;                       // tmp 位置右移
            }
            while (i < m) {                // 左半段還有剩餘
                tmp[k++] = a[i++];         // 直接補進 tmp
            }
            while (j < r) {                // 右半段還有剩餘
                tmp[k++] = a[j++];         // 直接補進 tmp
            }
            for (int x = l; x < r; x++) {  // 將 tmp 複製回原陣列
                a[x] = tmp[x];             // 複製一格
            }
        }
    }
}

void down(vector<int>& a, int s, int e) { // Heap Sort 的下沉調整
    int root = s;                         // 從指定節點開始
    while (root * 2 + 1 <= e) {           // 只要還有左子節點
        int lc = root * 2 + 1;            // 左子節點
        int rc = lc + 1;                  // 右子節點
        int big = root;                   // 先假設 root 最大
        if (a[big] < a[lc]) {             // 如果左子節點較大
            big = lc;                     // 最大位置改成左子節點
        }
        if (rc <= e && a[big] < a[rc]) {  // 如果右子節點存在且較大
            big = rc;                     // 最大位置改成右子節點
        }
        if (big == root) {                // 如果 root 已經最大
            return;                       // 不需要再調整
        }
        swap(a[root], a[big]);            // 將較大的子節點換上來
        root = big;                       // 繼續往下檢查
    }
}

void heap(vector<int>& a) {                    // Heap Sort 堆積排序
    int n = (int)a.size();                     // 取得資料大小
    for (int s = n / 2 - 1; s >= 0; s--) {     // 從最後一個非葉節點開始
        down(a, s, n - 1);                     // 建立 max heap
    }
    for (int e = n - 1; e > 0; e--) {          // 逐步縮小 heap 範圍
        swap(a[0], a[e]);                      // 將最大值放到最後
        down(a, 0, e - 1);                     // 重新調整 heap
    }
}

void cmp(vector<int>& a) { // Composite Sort 組合排序
    int n = (int)a.size(); // 取得資料大小
    if (n <= 32) {         // 小資料量
        ins(a);            // 使用插入排序
    }
    else {               // 大資料量
        mrg(a);            // 使用合併排序
    }
}

void run(ST t, vector<int>& a) {  // 根據排序種類執行排序
    if (t == INS) ins(a);         // 執行插入排序
    else if (t == QCK) qck(a);    // 執行快速排序
    else if (t == MRG) mrg(a);    // 執行合併排序
    else if (t == HEP) heap(a);   // 執行堆積排序
    else if (t == CMP) cmp(a);    // 執行組合排序
}

bool ok(ST t) {                         // 檢查排序是否正確
    vector<vector<int>> tests = {       // 建立測試資料
        {},                             // 空陣列
        {1},                            // 一筆資料
        {2, 1},                         // 兩筆反向資料
        {5, 1, 3, 3, 2, 9, 0},          // 含重複值資料
        rev(100),                       // 反向資料
        rnd(257)                        // 隨機資料
    };
    for (vector<int> a : tests) {       // 逐一測試
        vector<int> ans = a;            // 複製一份作標準答案
        sort(ans.begin(), ans.end());   // 使用內建排序產生答案
        run(t, a);                      // 使用自己的排序法
        if (a != ans) {                 // 如果結果不同
            return false;               // 排序錯誤
        }
    }
    return true;                        // 全部通過
}

double rpt(ST t, const vector<int>& src, int& times) { // 重複計時
    times = 1;                                         // 初始重複次數
    double total = 0.0;                                // 總時間
    while (times <= 1048576) {                         // 限制最大重複次數
        auto st = Clk::now();                          // 開始計時
        for (int i = 0; i < times; i++) {              // 重複排序
            vector<int> a = src;                       // 每次複製原始資料
            run(t, a);                                 // 執行排序
        }
        auto ed = Clk::now();                          // 結束計時
        total = Us(ed - st).count();                   // 轉成微秒
        if (total >= 100000.0) {                       // 總時間夠長時
            break;                                     // 停止加倍
        }
        times *= 2;                                    // 時間太短就加倍次數
    }
    return total / times;                              // 回傳平均時間
}

vector<int> mwRec(const vector<int>& s) {      // 產生 Merge Sort worst-case
    int n = (int)s.size();                     // 取得資料大小
    if (n <= 1) {                              // 如果只有 0 或 1 筆
        return s;                              // 直接回傳
    }
    vector<int> l;                             // 左半部
    vector<int> r;                             // 右半部
    for (int i = 0; i < n; i++) {              // 掃描資料
        if (i % 2 == 0) {                      // 偶數位置
            l.push_back(s[i]);                 // 放入左半
        }
        else {                               // 奇數位置
            r.push_back(s[i]);                 // 放入右半
        }
    }
    l = mwRec(l);                              // 遞迴處理左半
    r = mwRec(r);                              // 遞迴處理右半
    l.insert(l.end(), r.begin(), r.end());     // 合併左右結果
    return l;                                  // 回傳結果
}

vector<int> wdata(ST t, int n) { // 依排序法產生 worst-case 資料
    if (t == INS) {              // 插入排序
        return rev(n);           // 使用反向排列
    }
    if (t == MRG) {              // 合併排序
        return mwRec(seq(n));    // 使用反推 worst-case
    }
    return rnd(n);               // 其他排序用隨機排列近似
}

R worst(ST t, int n, int rt) {              // 測量 worst-case
    if (t == QCK || t == HEP || t == CMP) { // 較難直接產生 worst-case 的排序
        double mx = 0.0;                    // 紀錄最大時間
        for (int i = 0; i < rt; i++) {      // 測試 rt 次
            int times = 1;                  // 重複次數
            vector<int> a = rnd(n);         // 產生隨機排列
            double now = rpt(t, a, times);  // 測量時間
            mx = max(mx, now);              // 更新最大時間
        }
        return { n, nm(t), "worst", mx, rt }; // 回傳結果
    }
    int times = 1;                          // 重複次數
    vector<int> a = wdata(t, n);            // 產生 worst-case 資料
    double now = rpt(t, a, times);          // 測量時間
    return { n, nm(t), "worst", now, times }; // 回傳結果
}

R avg(ST t, int n, int rt) {                  // 測量 average-case
    vector<vector<int>> samples;              // 儲存隨機資料
    for (int i = 0; i < rt; i++) {            // 產生 rt 組資料
        samples.push_back(rnd(n));            // 加入隨機排列
    }
    auto st = Clk::now();                     // 開始計時
    for (vector<int> a : samples) {           // 逐一排序
        run(t, a);                            // 執行排序
    }
    auto ed = Clk::now();                     // 結束計時
    double mean = Us(ed - st).count() / rt;   // 計算平均時間
    return { n, nm(t), "average", mean, rt };   // 回傳結果
}

void show(const vector<R>& rs) {                    // 顯示結果
    cout << fixed << setprecision(3);               // 固定三位小數
    cout << "\nTiming summary, microseconds per sort\n"; // 顯示標題
    cout << "n,algorithm,data_kind,microseconds,trials\n"; // 顯示欄位
    for (R x : rs) {                                // 逐筆輸出
        cout << x.n << ","                          // 輸出 n
            << x.alg << ","                       // 輸出排序法
            << x.kind << ","                      // 輸出資料類型
            << x.us << ","                        // 輸出微秒
            << x.times << "\n";                   // 輸出次數
    }
}

int main() {                                                   // 主程式開始
    vector<ST> algs = { INS, QCK, MRG, HEP, CMP };               // 排序法清單
    vector<int> ns = { 500, 1000, 2000, 3000, 4000, 5000 };      // 測試資料量
    vector<R> rs;                                              // 儲存所有結果
    cout << "Timer: chrono::high_resolution_clock\n";          // 顯示計時器
    cout << "Clock accuracy estimate: "                        // 顯示計時精度
        << (double)Clk::period::num / Clk::period::den * 1000000.0 // 計算微秒精度
        << " microseconds per tick\n";                        // 顯示單位
    for (ST t : algs) {                                        // 檢查每種排序法
        if (!ok(t)) {                                          // 如果排序錯誤
            cout << nm(t) << " failed correctness test.\n";    // 顯示錯誤
            return 1;                                          // 結束程式
        }
    }
    cout << "Correctness tests passed.\n";                     // 正確性測試通過
    for (int n : ns) {                                         // 逐一測試不同 n
        int rt = max(10, 10000 / n);                           // random permutation 次數
        for (ST t : algs) {                                    // 逐一測試排序法
            rs.push_back(worst(t, n, rt));                     // 加入 worst-case 結果
            rs.push_back(avg(t, n, rt));                       // 加入 average-case 結果
        }
    }
    show(rs);                                                  // 顯示結果
    cout << "\nFinished. Copy these results into Excel or your report if needed.\n"; // 完成訊息
    return 0;                                                  // 正常結束
}
