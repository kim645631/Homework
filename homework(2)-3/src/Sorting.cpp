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
