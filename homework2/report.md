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
    Polynomial A, B;
    A.NewTerm(6, 5);
    A.NewTerm(4, 3);
    A.NewTerm(3, 0);
    B.NewTerm(2, 3);
    B.NewTerm(2, 3);

    cout << "P1(x) = "; A.Show();
    cout << "P2(x) = "; B.Show();

    Polynomial sum = A.Add(B);
    cout << "Sum = "; sum.Show();

    Polynomial product = A.Mult(B);
    cout << "Product = "; product.Show();

    cout << "P1(3) = " << A.Eval(3) << endl;
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
|   $A(x)$  +    $B(x)$     |    |  6x^5 + 8x^3 + 3       | 6x^5 + 8x^3 + 3         |   
|   $A(x)$ * $B(x)$     |   |  24x^8 + 16x^6 + 12x^3       |   24x^8 + 16x^6 + 12x^3              |
|   $A(3)$      |  | 1569    |   1569   |
#### 測試輸入
```cpp
A.NewTerm(6, 5);  
A.NewTerm(4, 3);  
A.NewTerm(3, 0);  
B.NewTerm(2, 3);  
B.NewTerm(2, 3);  
```
#### 測試輸出
```shell
A(x) = 6x^5 + 4x^3 + 3
B(x) = 4x^3
Sum = 6x^5 + 8x^3 + 3
Product = 24x^8 + 16x^6 + 12x^3
A(3) = 1569
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
using namespace std;

class Polynomial;

class Term {
    friend class Polynomial;
    friend ostream& operator<<(ostream&, const Polynomial&);
private:
    float coef;
    int exp;
};

class Polynomial {
    friend istream& operator>>(istream& in, Polynomial& poly);
    friend ostream& operator<<(ostream& out, const Polynomial& poly);

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
};

    istream& operator>>(istream& in, Polynomial& poly) {
        int n;
        cout << "Enter number of terms: ";
        in >> n;

        poly.terms = 0;

        for (int i = 0; i < n; i++) {
            float c;
            int e;
            cout << "Enter coefficient and exponent for term " << i + 1 << ": ";
            in >> c >> e;
            poly.NewTerm(c, e);
        }

        return in;
    }
    ostream& operator<<(ostream& out, const Polynomial& poly) {
        if (poly.terms == 0) {
            out << "0";
            return out;
        }

        for (int i = 0; i < poly.terms; i++) {
            float c = poly.termArray[i].coef;
            int e = poly.termArray[i].exp;

            if (i > 0) {
                if (c >= 0) out << " + ";
                else { out << " - "; c = -c; }
            }
            else if (c < 0) {
                out << "-";
                c = -c;
            }

            if (e == 0)
                out << c;
            else if (e == 1)
                out << c << "x";
            else
                out << c << "x^" << e;
        }

        return out;
    }

    int main() {
        Polynomial A,B;

        cin >> A;
        cin >> B;
        Polynomial sum = A.Add(B);
        Polynomial prod = A.Mult(B);
        float x;
        cout << "Enter x value: ";
        cin >> x;
        cout << "\nA(x) = " << A << endl;
        cout << "B(x) = " << B << endl;
        cout << "Sum = " << sum << endl;
        cout << "Product = " << prod << endl;

        cout << "p(" << x << ") = " << A.Eval(x) << endl;

        return 0;
    }
```
## 效能分析

## 測試與驗證
| 測試案例 | 輸入參數   | 預期輸出  | 實際輸出  | 
|----------|--------------|----------|----------|
| 測試一   |      3     |             |         |       
|          |        3, 2    |            |        | 
|          |        2,1     |            |        |
|          |        1, 0    |    3x^2 + 2x + 1       |    3x^2 + 2x + 1      |  
| 測試二  |      2    |             |         |       
|          |       2, 5    |            |        | 
|          |       3, 1    |       2x^5 + 3x    |   2x^5 + 3x     |
|   $A(x)$  +    $B(x)$     |    |  2x^5 + 3x^2 + 5x + 1       | 2x^5 + 3x^2 + 5x + 1        |   
|   $A(x)$ * $B(x)$     |   |  6x^7 + 4x^6 + 2x^5 + 9x^3 + 6x^2 + 3x       |   6x^7 + 4x^6 + 2x^5 + 9x^3 + 6x^2 + 3x              |
|   $A(2)$      |  | 17    |   17  |
#### 測試輸入
```cpp
Enter number of terms: 3
Enter coefficient and exponent for term 1: 3 2
Enter coefficient and exponent for term 2: 2 1
Enter coefficient and exponent for term 3: 1 0
Enter number of terms: 2
Enter coefficient and exponent for term 1: 2 5
Enter coefficient and exponent for term 2: 3 1
Enter x value: 2 
```
#### 測試輸出
```shell
A(x) = 3x^2 + 2x + 1
B(x) = 2x^5 + 3x
Sum = 2x^5 + 3x^2 + 5x + 1
Product = 6x^7 + 4x^6 + 2x^5 + 9x^3 + 6x^2 + 3x
p(2) = 17
```

## 申論及開發報告
