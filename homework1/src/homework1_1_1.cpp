#include <iostream>
using namespace std;
int ack(int m, int n) {
    if (m == 0) return n + 1; //if m=0,n+1
    else if (n == 0) return ack(m - 1, 1); //if n=0,A(m-1,n)
    else return ack(m - 1, ack(m, n - 1)); //otherwise , A(m-1,A(m,n-1))
}
int main() {
    int m, n;
    cout << "輸入 m 和 n : ";
    while (cin >> m >> n) {
        cout << "(" << m << ", " << n << ") = " << ack(m, n) << endl;
        cout << "輸入 m 和 n : ";
    }
    return 0;
}
