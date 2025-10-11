#include <iostream>
#include <cstring> // for memset

int ackermann(int m, int n) {
    // 設定一個足夠大的 stack 來模擬遞迴
    const int MAX_STACK = 100000; // 可依需求調整
    int stack[MAX_STACK];
    int top = 0;
    stack[top++] = m;

    while (top > 0) {
        m = stack[--top];
        if (m == 0) {
            if (top == 0) {
                return n + 1;
            }
            n = n + 1;
        } else if (n == 0) {
            stack[top++] = m - 1;
            n = 1;
        } else {
            stack[top++] = m - 1;
            stack[top++] = m;
            n = n - 1;
        }
    }
    return n;
}

int main() {
    int m, n;
    std::cout << "Enter m and n: ";
    std::cin >> m >> n;
    std::cout << "Ackermann(" << m << ", " << n << ") = " << ackermann(m, n) << std::endl;
    return 0;
}
