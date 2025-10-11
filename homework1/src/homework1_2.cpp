#include <iostream>
#include <string>
using namespace std;

void pow(string A[], string s[], int i, int n, int t) {    // A[]:原集合 s[]:當前子集 i:目前處理到A[i] n:集合大小 t:當前子集大小
    if (i == n) {
        cout << "{";
        for (int j = 0; j < t; j++) {
            cout << s[j];
            if (j != t - 1) cout << ",";
        }
        cout << "}" << endl;
        return;
    }
    pow(A, s, i + 1, n, t);         // 不加入當前元素 A[i]，繼續處理下一個元素
    s[t] = A[i];                    // 加入當前元素 A[i] 到子集
    pow(A, s, i + 1, n, t + 1);     // 繼續處理下一個元素，子集大小 t 增加 1
}

int main() {
    int n;
    cout << "請輸入n個數 : ";
    cin >> n;
    string* A = new string[n];
    string* s = new string[n];
    cout << "請輸入集合元素 : ";
    for (int i = 0; i < n; i++) {
        cin >> A[i];
    }
    cout << "集合的冪集為:" << endl;
    pow(A, s, 0, n, 0);

    delete[] A;
    delete[] s;

    return 0;
}
