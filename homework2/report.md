# 41343121

## 作業二 之Polynomial 1
## 解題說明

### 解題策略

## 程式實作
### IDE:
Microsoft Visual Studio Code C/C++
```cpp
#include <iostream>
#include <cmath>
using namespace std;

class Polynomial;

class Term {
    friend class Polynomial;
private:
    float coef;
    int exp;
};

class Polynomial {
private:
    Term* termArray;
    int capacity;
    int terms;

public:
    Polynomial() {
        capacity = 10;
        terms = 0;
        termArray = new Term[capacity];
    }

    Polynomial(const Polynomial& poly) {
        capacity = poly.capacity;
        terms = poly.terms;
        termArray = new Term[capacity];
        for (int i = 0; i < terms; i++)
            termArray[i] = poly.termArray[i];
    }

    void NewTerm(float newCoef, int newExp) {
        if (newCoef == 0) return;
        for (int i = 0; i < terms; i++) {
            if (termArray[i].exp == newExp) {
                termArray[i].coef += newCoef;
                if (termArray[i].coef == 0) {
                    for (int j = i; j < terms - 1; j++)
                        termArray[j] = termArray[j + 1];
                    terms--;
                }
                return;
            }
        }
        if (terms == capacity) {
            capacity *= 2;
            Term* temp = new Term[capacity];
            for (int i = 0; i < terms; i++)
                temp[i] = termArray[i];
            delete[] termArray;
            termArray = temp;
        }
        termArray[terms].coef = newCoef;
        termArray[terms].exp = newExp;
        terms++;

        for (int i = 0; i < terms - 1; i++) {
            for (int j = i + 1; j < terms; j++) {
                if (termArray[i].exp < termArray[j].exp)
                    swap(termArray[i], termArray[j]);
            }
        }
    }

    Polynomial Add(Polynomial poly) {
        Polynomial result;
        int aPos = 0, bPos = 0;
        while (aPos < terms && bPos < poly.terms) {
            if (termArray[aPos].exp == poly.termArray[bPos].exp) {
                float sum = termArray[aPos].coef + poly.termArray[bPos].coef;
                if (sum != 0)
                    result.NewTerm(sum, termArray[aPos].exp);
                aPos++; bPos++;
            }
            else if (termArray[aPos].exp > poly.termArray[bPos].exp) {
                result.NewTerm(termArray[aPos].coef, termArray[aPos].exp);
                aPos++;
            }
            else {
                result.NewTerm(poly.termArray[bPos].coef, poly.termArray[bPos].exp);
                bPos++;
            }
        }
        for (; aPos < terms; aPos++)
            result.NewTerm(termArray[aPos].coef, termArray[aPos].exp);
        for (; bPos < poly.terms; bPos++)
            result.NewTerm(poly.termArray[bPos].coef, poly.termArray[bPos].exp);
        return result;
    }

    Polynomial Mult(Polynomial poly) {
        Polynomial result;
        for (int i = 0; i < terms; i++) {
            Polynomial temp;
            for (int j = 0; j < poly.terms; j++) {
                float newCoef = termArray[i].coef * poly.termArray[j].coef;
                int newExp = termArray[i].exp + poly.termArray[j].exp;
                temp.NewTerm(newCoef, newExp);
            }
            result = result.Add(temp);
        }
        return result;
    }

    float Eval(float f) {
        float result = 0;
        for (int i = 0; i < terms; i++)
            result += termArray[i].coef * pow(f, termArray[i].exp);
        return result;
    }

    void Show() {
        if (terms == 0) {
            cout << "0" << endl;
            return;
        }
        for (int i = 0; i < terms; i++) {
            float c = termArray[i].coef;
            int e = termArray[i].exp;
            if (i > 0) {
                if (c >= 0) cout << " + ";
                else {
                    cout << " - ";
                    c = -c;
                }
            }
            else {
                if (c < 0) {
                    cout << "-";
                    c = -c;
                }
            }
            if (e == 0)
                cout << c;
            else if (e == 1)
                cout << c << "x";
            else
                cout << c << "x^" << e;
        }
        cout << endl;
    }
};

int main() {
    Polynomial p1, p2;
    p1.NewTerm(3, 2);
    p1.NewTerm(2, 1);
    p1.NewTerm(1, 0);
    p2.NewTerm(1, 1);
    p2.NewTerm(1, 1);

    cout << "P1(x) = "; p1.Show();
    cout << "P2(x) = "; p2.Show();

    Polynomial sum = p1.Add(p2);
    cout << "Sum = "; sum.Show();

    Polynomial product = p1.Mult(p2);
    cout << "Product = "; product.Show();

    cout << "P1(2) = " << p1.Eval(2) << endl;
    return 0;
}
```
## 效能分析


## 測試與驗證
| 測試案例 | 輸入參數   | 預期輸出  | 實際輸出  | 
|----------|--------------|----------|----------|
| 測試一   |      6, 5     |             |         |       
|          |        4, 3    |            |        |  
|          |        3, 0    |    6x^5 + 4x^3 + 3       |    6x^5 + 4x^3 + 3      |  
| 測試二  |      2, 3    |             |         |       
|          |       2, 3    |       4x^3     |   4x^3      | 
|   $p1(x)$  +    $p2(x)$     |    6x^5 + 8x^3 + 3         |  6x^5 + 8x^3 + 3       |   
|   $p1(x)$ * $p2(x)$     |   24x^8 + 16x^6 + 12x^3       |  24x^8 + 16x^6 + 12x^3       |   
|   $p1(3)$      |  1569   | 1569    |   
#### 測試輸入
```cpp
p1.NewTerm(6, 5);  
p1.NewTerm(4, 3);  
p1.NewTerm(3, 0);  
p2.NewTerm(2, 3);  
p2.NewTerm(2, 3);  
```
#### 測試輸出
```shell
P1(x) = 6x^5 + 4x^3 + 3
P2(x) = 4x^3
Sum = 6x^5 + 8x^3 + 3
Product = 24x^8 + 16x^6 + 12x^3
P1(3) = 1569
```
## 申論及開發報告
--------------------------------------------------------------------------------------------------------------------------------------------------------------------
## 作業二 之Polynomial 2
## 解題說明
## 程式實作
### IDE:
Microsoft Visual Studio Code C/C++
```cpp
#include <iostream>
#include <cmath>
#include <algorithm>
using namespace std;

class Polynomial; // forward declaration

class Term {
    friend Polynomial;
    friend ostream& operator<<(ostream& output, const Polynomial& Poly);
private:
    float coef; // 系數
    int exp;    // 指數
};

class Polynomial {
private:
    Term* termArray;  // 儲存非零項的動態陣列
    int capacity;     // 陣列大小
    int terms;        // 非零項數量

public:
    // p(x) = 0
    Polynomial() : capacity(10), terms(0) {
        termArray = new Term[capacity];
    }
    ~Polynomial() {
        delete[] termArray;
    }

    void NewTerm(const float c, const int e) {
        if (c == 0) return;
        if (terms == capacity) {
            capacity *= 2;
            Term* temp = new Term[capacity];
            copy(termArray, termArray + terms, temp);
            delete[] termArray;
            termArray = temp;
        }
        termArray[terms].coef = c;
        termArray[terms++].exp = e;
    }

    Polynomial operator+(const Polynomial& poly) const {
        Polynomial result;
        int a = 0, b = 0;
        while (a < terms && b < poly.terms) {
            if (termArray[a].exp == poly.termArray[b].exp) {
                float sum = termArray[a].coef + poly.termArray[b].coef;
                if (sum != 0)
                    result.NewTerm(sum, termArray[a].exp);
                a++; b++;
            }
            else if (termArray[a].exp > poly.termArray[b].exp) {
                result.NewTerm(termArray[a].coef, termArray[a].exp);
                a++;
            }
            else {
                result.NewTerm(poly.termArray[b].coef, poly.termArray[b].exp);
                b++;
            }
        }
        for (; a < terms; a++)
            result.NewTerm(termArray[a].coef, termArray[a].exp);
        for (; b < poly.terms; b++)
            result.NewTerm(poly.termArray[b].coef, poly.termArray[b].exp);
        return result;
    }

    Polynomial operator*(const Polynomial& poly) const {
        Polynomial result;
        for (int i = 0; i < terms; i++) {
            for (int j = 0; j < poly.terms; j++) {
                float c = termArray[i].coef * poly.termArray[j].coef;
                int e = termArray[i].exp + poly.termArray[j].exp;
                bool found = false;
                for (int k = 0; k < result.terms; k++) {
                    if (result.termArray[k].exp == e) {
                        result.termArray[k].coef += c;
                        found = true;
                        break;
                    }
                }
                if (!found)
                    result.NewTerm(c, e);
            }
        }
        sort(result.termArray, result.termArray + result.terms,
            [](Term a, Term b) { return a.exp > b.exp; });
        return result;
    }

    float operator()(float x) const {
        float sum = 0;
        for (int i = 0; i < terms; i++)
            sum += termArray[i].coef * pow(x, termArray[i].exp);
        return sum;
    }

    friend istream& operator>>(istream& input, Polynomial& Poly) {
        int n;
        float coef;
        int exp;
        input >> n;
        for (int i = 0; i < n; i++) {
            input >> coef >> exp;
            Poly.NewTerm(coef, exp);
        }
        return input;
    }

    friend ostream& operator<<(ostream& os, const Polynomial& poly) {
        if (poly.terms == 0) {
            os << "0";
            return os;
        }
        for (int i = 0; i < poly.terms; i++) {
            if (i > 0 && poly.termArray[i].coef > 0)
                os << " + ";
            else if (poly.termArray[i].coef < 0)
                os << " ";
            os << poly.termArray[i].coef;
            if (poly.termArray[i].exp != 0)
                os << "x^" << poly.termArray[i].exp;
        }
        return os;
    }
};

int main() {
    Polynomial a, b;

    cout << "多項式 a(x):" << endl;
    cin >> a;
    cout << "\n多項式 b(x):" << endl;
    cin >> b;

    cout << "\na(x) = " << a << endl;
    cout << "b(x) = " << b << endl;

    cout << "\na + b = " << a + b << endl;
    cout << "a * b = " << a * b << endl;

    float x;
    cout << "\n要代入的 x ：";
    cin >> x;
    cout << "a(" << x << ") = " << a(x) << endl;
    cout << "b(" << x << ") = " << b(x) << endl;

    return 0;
}

```
## 效能分析
## 測試與驗證
| 測試案例 | 輸入參數 $p1(x)$  | 預期輸出P1(x)  | 實際輸出 P1(x) | 
|----------|--------------|----------|----------|
| 測試一   |      3, 2     |             |         |       
|          |        2, 1    |            |        |  
|          |        1, 0    |    3x^2 + 2x + 1        |    3x^2 + 2x + 1      |  

| 測試案例 | 輸入參數 $p2(x)$  | 預期輸出P2(x)  | 實際輸出 P2(x) | 
|----------|--------------|----------|----------|
| 測試二  |      1, 1    |             |         |       
|          |       1, 1    |      2x      |  2x      | 

| 測試案例 |   預期輸出$p1(x)$ +    $p2(x)$  | 實際輸出 $p1(x)$ +    $p2(x)$ | 
|----------|----------|----------|
|   $p1(x)$ +    $p2(x)$     |    3x^2 + 4x + 1         |  3x^2 + 4x + 1       |   

| 測試案例 |   預期輸出$p1(x)$ * $p2(x)$  | 實際輸出 $p1(x)$ * $p2(x)$ | 
|----------|----------|----------|
|   $p1(x)$ * $p2(x)$     |    6x^3 + 4x^2 + 2x       |  6x^3 + 4x^2 + 2x       |   

| 測試案例 |   預期輸出$p1(2)$  | 實際輸出 $p1(2)$ | 
|----------|----------|----------|
|   $p1(2)$      |   17    | 17     |  
### 輸入

多項式 a(x):

    3

    2 3

    2 2

    4 1

多項式 b(x):

    2

    3 1

    4 0
    
要代入x ：
      
    2

### 則輸出

    a(x) = 2x^3 + 2x^2 + 4x^1

    b(x) = 3x^1 + 4

    a + b = 2x^3 + 2x^2 + 7x^1 + 4

    a * b = 6x^4 + 14x^3 + 20x^2 + 16x^1

    a(2) = 32

    b(2) = 10

## 申論及開發報告
