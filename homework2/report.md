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

    Polynomial Add(Polynomial Poly) {
        Polynomial result;
        int a = 0, b = 0;
        while (a < terms && b < Poly.terms) {
            if (termArray[a].exp == Poly.termArray[b].exp) {
                float sum = termArray[a].coef + Poly.termArray[b].coef;
                if (sum != 0)
                    result.Newterms(sum, termArray[a].exp);
                a++; b++;
            }
            else if (termArray[a].exp > Poly.termArray[b].exp) {
                result.Newterms(termArray[a].coef, termArray[a].exp);
                a++;
            }
            else {
                result.Newterms(Poly.termArray[b].coef, Poly.termArray[b].exp);
                b++;
            }
        }
        for (; a < terms; a++)
            result.Newterms(termArray[a].coef, termArray[a].exp);
        for (; b < Poly.terms; b++)
            result.Newterms(Poly.termArray[b].coef, Poly.termArray[b].exp);
        return result;
    }

    Polynomial Mult(Polynomial Poly) {
        Polynomial result;
        for (int i = 0; i < terms; i++) {
            Polynomial temp;
            for (int j = 0; j < Poly.terms; j++) {
                float newCoef = termArray[i].coef * Poly.termArray[j].coef;
                int newExp = termArray[i].exp + Poly.termArray[j].exp;
                temp.Newterms(newCoef, newExp);
            }
            result = result.Add(temp);
        }
        return result;
    }

    float Eval(float Float) {
        float result = 0;
        for (int i = 0; i < terms; i++)
            result += termArray[i].coef * pow(Float, termArray[i].exp);
        return result;
    }
    void Newterms(float newCoef, int newExp) {
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
    void Exhibit() {
        if (terms == 0) {
            cout << "0" << endl;
            return;
        }
        for (int i = 0; i < terms; i++) {
            float Coef = termArray[i].coef;
            int Exp = termArray[i].exp;
            if (i > 0) {
                if (Coef >= 0) cout << " + ";
                else {
                    cout << " - ";
                    Coef = -Coef;
                }
            }
            else {
                if (Coef < 0) {
                    cout << "-";
                    Coef = -Coef;
                }
            }
            if (Exp == 0)
                cout << Coef;
            else if (Exp == 1)
                cout << Coef << "x";
            else
                cout << Coef << "x^" << Exp;
        }
        cout << endl;
    }
};

int main() {
    Polynomial A, B;
    A.Newterms(6, 5);
    A.Newterms(4, 3);
    A.Newterms(3, 0);
    B.Newterms(2, 3);
    B.Newterms(2, 3);

    cout << "A(x) = "; A.Exhibit();
    cout << "B(x) = "; B.Exhibit();

    Polynomial sum = A.Add(B);
    cout << "Sum = "; sum.Exhibit();

    Polynomial product = A.Mult(B);
    cout << "Product = "; product.Exhibit();

    cout << "A(3) = " << A.Eval(3) << endl;
    return 0;
}
```
## 效能分析


## 測試與驗證
### 測試案例
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
### 測試輸入
```cpp
A.Newterms(6, 5);
A.Newterms(4, 3);
A.Newterms(3, 0);
B.Newterms(2, 3);
B.Newterms(2, 3);
```
### 測試輸出
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
    friend ostream& operator<<(ostream& out, const Polynomial& Poly);
    friend istream& operator>>(istream& in, Polynomial& Poly);
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

    Polynomial Add(Polynomial Poly) {
        Polynomial result;
        int a = 0, b = 0;
        while (a < terms && b < Poly.terms) {
            if (termArray[a].exp == Poly.termArray[b].exp) {
                float sum = termArray[a].coef + Poly.termArray[b].coef;
                if (sum != 0)
                    result.Newterms(sum, termArray[a].exp);
                a++; b++;
            }
            else if (termArray[a].exp > Poly.termArray[b].exp) {
                result.Newterms(termArray[a].coef, termArray[a].exp);
                a++;
            }
            else {
                result.Newterms(Poly.termArray[b].coef, Poly.termArray[b].exp);
                b++;
            }
        }
        for (; a < terms; a++)
            result.Newterms(termArray[a].coef, termArray[a].exp);
        for (; b < Poly.terms; b++)
            result.Newterms(Poly.termArray[b].coef, Poly.termArray[b].exp);
        return result;
    }

    Polynomial Mult(Polynomial Poly) {
        Polynomial result;
        for (int i = 0; i < terms; i++) {
            Polynomial temp;
            for (int j = 0; j < Poly.terms; j++) {
                float newCoef = termArray[i].coef * Poly.termArray[j].coef;
                int newExp = termArray[i].exp + Poly.termArray[j].exp;
                temp.Newterms(newCoef, newExp);
            }
            result = result.Add(temp);
        }
        return result;
    }

    float Eval(float Float) {
        float result = 0;
        for (int i = 0; i < terms; i++)
            result += termArray[i].coef * pow(Float, termArray[i].exp);
        return result;
    }
    void Newterms(float newCoef, int newExp) {
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
};

istream& operator>>(istream& In, Polynomial& Poly) {
    int n;
    cout << "輸入次數: ";
    In >> n;

    Poly.terms = 0;

    for (int i = 0; i < n; i++) {
        float Coef;
        int Exp;
        cout << "輸入第"<< i + 1<<"項的係數和指數 "  << ": ";
        In >> Coef >> Exp;
        Poly.Newterms(Coef, Exp);
    }

    return In;
}
ostream& operator<<(ostream& Out, const Polynomial& Poly) {
    if (Poly.terms == 0) {
        Out << "0";
        return Out;
    }

    for (int i = 0; i < Poly.terms; i++) {
        float Coef = Poly.termArray[i].coef;
        int Exp = Poly.termArray[i].exp;

        if (i > 0) {
            if (Coef >= 0) Out << " + ";
            else { Out << " - "; Coef = -Coef; }
        }
        else if (Coef < 0) {
            Out << "-";
            Coef = -Coef;
        }

        if (Exp == 0)
            Out << Coef;
        else if (Exp == 1)
            Out << Coef << "x";
        else
            Out << Coef << "x^" << Exp;
    }

    return Out;
}

int main() {
    Polynomial A, B;

    cin >> A;
    cin >> B;
    Polynomial sum = A.Add(B);
    Polynomial prod = A.Mult(B);
    float x;
    cout << "輸入x為多少: ";
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
### 測試案例
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
### 測試輸入
```cpp
輸入次數: 3
輸入第1項的係數和指數 : 3 2
輸入第2項的係數和指數 : 2 1
輸入第3項的係數和指數 : 1 0
輸入次數: 2
輸入第1項的係數和指數 : 2 5
輸入第2項的係數和指數 : 3 1
輸入x為多少: 2
```
### 測試輸出
```shell
A(x) = 3x^2 + 2x + 1
B(x) = 2x^5 + 3x
Sum = 2x^5 + 3x^2 + 5x + 1
Product = 6x^7 + 4x^6 + 2x^5 + 9x^3 + 6x^2 + 3x
p(2) = 17
```

## 申論及開發報告


