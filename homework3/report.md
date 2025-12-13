# 41343121
## 作業三 Polynomial+Available Lists
## 解題說明

### 解題策略

## 程式實作
### IDE:
Microsoft Visual Studio Code C/C++

```cpp
#include <iostream>
#include <cstdlib>
using namespace std;

template <class T>
class ChainNode;

template <class T>
class ChainIterator;

template <class T>
class Chain;

template <class T>
class AvailableList;

template <class T>
class ChainNode {
    friend class Chain<T>;
    friend class ChainIterator<T>;
    friend class AvailableList<T>;
private:
    T element;
    ChainNode<T>* next;
public:
    ChainNode() : next(nullptr) {}
    ChainNode(const T& elem) : element(elem), next(nullptr) {}
    ChainNode(const T& elem, ChainNode<T>* nextNode)
        : element(elem), next(nextNode) {}
    void setElement(const T& elem) {
        element = elem;
    }
    void setNext(ChainNode<T>* nextNode) {
        next = nextNode;
    }
};

// Iterator for Chain
template <class T>
class ChainIterator {
private:
    ChainNode<T>* currentNode;
public:
    ChainIterator(ChainNode<T>* startNode = nullptr) : currentNode(startNode) {}

    T& operator*() const { return currentNode->element; }
    T* operator->() const { return &currentNode->element; }
    
    ChainIterator& operator++() {
        if (currentNode) currentNode = currentNode->next;
        return *this;
    }

    ChainIterator operator++(int) {
        ChainIterator old = *this;
        if (currentNode) currentNode = currentNode->next;
        return old;
    }

    bool operator!=(const ChainIterator& right) const {
        return currentNode != right.currentNode;
    }
    bool operator==(const ChainIterator& right) const {
        return currentNode == right.currentNode;
    }

    int operator-(const ChainIterator& right) const {
        int distance = 0;
        ChainNode<T>* temp = right.currentNode;
        while (temp != currentNode) {
            if (temp == nullptr) {
                return -1;
            }
            temp = temp->next;
            ++distance;
        }
        return distance;
    }
};


template <class T>
class AvailableList {
private:
    static ChainNode<T>* availableHead;
public:
    AvailableList() {}
    ~AvailableList() {
       
        while (availableHead) {
            ChainNode<T>* tmp = availableHead;
            availableHead = availableHead->next;
            delete tmp;
        }
    }

    
    static ChainNode<T>* getNode() {
        if (!availableHead) return nullptr;
        ChainNode<T>* nodeToReturn = availableHead;
        availableHead = availableHead->next;
        nodeToReturn->next = nullptr;
        return nodeToReturn;
    }

   
    static void getBack(ChainNode<T>* firstNode) {
        if (!firstNode) return;
       
        ChainNode<T>* current = firstNode;
        while (current->next) {
            current = current->next;
        }
        
        current->next = availableHead;
        
        availableHead = firstNode;
    }

    static bool isEmpty() {
        return availableHead == nullptr;
    }

    
    static ChainNode<T>* getOneNodeOrNew(const T& elem) {
        ChainNode<T>* node = getNode();
        if (!node) node = new ChainNode<T>(elem);
        else node->setElement(elem);
        return node;
    }
};

template <class T>
ChainNode<T>* AvailableList<T>::availableHead = nullptr;

template <class T>
class Chain {
    friend class ChainIterator<T>;
    friend class AvailableList<T>;
private:
    ChainNode<T>* head;
public:
    Chain() : head(nullptr) {}
    ~Chain() {
        ChainNode<T>* current = head;
        while (current != nullptr) {
            ChainNode<T>* nextNode = current->next;
            delete current;
            current = nextNode;
        }
        head = nullptr;
    }

    ChainNode<T>* release() {
        ChainNode<T>* oldHead = head;
        head = nullptr;
        return oldHead;
    }

    ChainIterator<T> begin() const {
        return ChainIterator<T>(head);
    }
    ChainIterator<T> end() const {
        return ChainIterator<T>(nullptr);
    }

    void insert(int idx, const T& element) {
        ChainNode<T>* newNode = AvailableList<T>::getNode();
        if (!newNode) {
            newNode = new ChainNode<T>(element);
        }
        else {
            newNode->setElement(element);
        }

        if (idx == 0) {
            newNode->next = head;
            head = newNode;
        }
        else {
            ChainNode<T>* prev = head;

            for (int i = 0; i < idx - 1 && prev != nullptr; ++i) {
                prev = prev->next;
            }
            if (prev != nullptr) {
                newNode->next = prev->next;
                prev->next = newNode;
            }
            else {

                AvailableList<T>::getBack(newNode);
            }
        }
    }
};

struct Term {
    double coef;
    int exp;
    Term() : coef(0.0), exp(0) {}
    Term(double c, int e) : coef(c), exp(e) {}
};

AvailableList<Term> globalASL;

class Polynomial {
    friend ostream& operator<<(std::ostream& os, const Polynomial& poly);
    friend istream& operator>>(std::istream& is, Polynomial& poly);
private:
    Chain<Term> terms; 
public:
    Polynomial() {}

    // Copy constructor
    Polynomial(const Polynomial& other) {
        int index = 0;
        for (ChainIterator<Term> it = other.begin(); it != other.end(); ++it, ++index) {
            terms.insert(index, *it);
        }
    }

    Polynomial& operator=(const Polynomial& other) {
        if (this != &other) {

            AvailableList<Term>::getBack(terms.release());

            int index = 0;
            for (ChainIterator<Term> it = other.begin(); it != other.end(); ++it, ++index) {
                terms.insert(index, *it);
            }
        }
        return *this;
    }

    ~Polynomial() {
        AvailableList<Term>::getBack(terms.release());
    }

    ChainIterator<Term> begin() const {
        return terms.begin();
    }
    ChainIterator<Term> end() const {
        return terms.end();
    }

    void newTerm(double coef, int exp) {

        if (terms.begin() == terms.end()) {
            terms.insert(0, Term(coef, exp));
            return;
        }

        int index = 0;
        for (ChainIterator<Term> it = terms.begin(); it != terms.end(); ++it, ++index) {
            if (it->exp < exp) {

                terms.insert(index, Term(coef, exp));
                return;
            }
            else if (it->exp == exp) {

                it->coef += coef;
                return;
            }
        }

        terms.insert(index, Term(coef, exp));
    }

    Polynomial operator+(const Polynomial& other) const {
        Polynomial result;
        ChainIterator<Term> it1 = this->begin();
        ChainIterator<Term> it2 = other.begin();
        while (it1 != this->end() && it2 != other.end()) {
            if (it1->exp > it2->exp) {
                result.newTerm(it1->coef, it1->exp);
                ++it1;
            }
            else if (it1->exp < it2->exp) {
                result.newTerm(it2->coef, it2->exp);
                ++it2;
            }
            else {
                double newCoef = it1->coef + it2->coef;
                if (newCoef != 0) {
                    result.newTerm(newCoef, it1->exp);
                }
                ++it1;
                ++it2;
            }
        }
        while (it1 != this->end()) {
            result.newTerm(it1->coef, it1->exp);
            ++it1;
        }
        while (it2 != other.end()) {
            result.newTerm(it2->coef, it2->exp);
            ++it2;
        }
        return result;
    }

    Polynomial operator-(const Polynomial& other) const {
        Polynomial negOther;
        for (ChainIterator<Term> it = other.begin(); it != other.end(); ++it) {
            negOther.newTerm(-it->coef, it->exp);
        }
        return (*this) + negOther;
    }

    Polynomial operator*(const Polynomial& other) const {
        Polynomial result;
        for (ChainIterator<Term> it1 = this->begin(); it1 != this->end(); ++it1) {
            for (ChainIterator<Term> it2 = other.begin(); it2 != other.end(); ++it2) {
                double newCoef = it1->coef * it2->coef;
                int newExp = it1->exp + it2->exp;
                result.newTerm(newCoef, newExp);
            }
        }
        return result;
    }
    float Evaluate(float x) const {
        float result = 0.0f;

        for (ChainIterator<Term> it = begin(); it != end(); ++it) {
            float termValue = it->coef;
            int exp = it->exp;


            float power = 1.0f;
            for (int i = 0; i < exp; ++i) {
                power *= x;
            }

            result += termValue * power;
        }
        return result;
    }
};

istream& operator>>(std::istream& is, Polynomial& poly) {
    int numTerms;
    if (!(is >> numTerms)) return is;
    for (int i = 0; i < numTerms; ++i) {
        double coef;
        int exp;
        std::cout << "輸入第"<<i+1<<"的指數、係數:";
        is >> coef >> exp;
        poly.newTerm(coef, exp);
    }
    return is;
}

ostream& operator<<(std::ostream& os, const Polynomial& poly) {
    bool first = true;
    for (ChainIterator<Term> it = poly.begin(); it != poly.end(); ++it) {
        if (!first) {
            os << " + ";
        }
        first = false;
        os << it->coef << "x^" << it->exp;
    }
    return os;
}

int main() {
    Polynomial A, B;
    float x;
    cout << "輸入A的項數:";
    cin >> A;
    cout << "輸入B的項數:";
    cin >> B;
    cout << "輸入x為多少:";
    cin >> x;

    cout << "A = " << A << '\n';
    cout << "B = " << B << '\n';

    cout << "A + B = " << (A + B) << '\n';
    cout << "A - B = " << (A - B) << '\n';
    cout << "A * B = " << (A * B) << '\n';
    
    

    cout << "A(" << x << ") = " << A.Evaluate(x) << '\n';

    return 0;
}


```
## 效能分析
1. 時間複雜度：

2. 空間複雜度：


## 測試與驗證
### 測試案例

| 測試案例 | 輸入參數   | 預期輸出  | 實際輸出  | 
|----------|--------------|----------|----------|
| 測試一   |      3     |             |         |       
|          |        3, 2    |            |        | 
|          |        2,1     |            |        |
|          |        1, 0    |    3x^2 + 2x + 1       |    3x^2 + 2x + 1      |  
| 測試二  |      2    |            |         |       
|          |       2, 5    |            |        | 
|          |       3, 1    |       2x^5 + 3x    |   2x^5 + 3x     |
|   $A(x)$  +    $B(x)$     |      |  2x^5 + 3x^2 + 5x + 1       | 2x^5 + 3x^2 + 5x + 1        |   
|   $A(x)$ * $B(x)$  |     |  6x^7 + 4x^6 + 2x^5 + 9x^3 + 6x^2 + 3x       |   6x^7 + 4x^6 + 2x^5 + 9x^3 + 6x^2 + 3x              |
|   $A(2)$      |      | 17    |   17  |

### 測試輸入
```
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
```
A(x) = 3x^2 + 2x + 1
B(x) = 2x^5 + 3x
Sum = 2x^5 + 3x^2 + 5x + 1
Product = 6x^7 + 4x^6 + 2x^5 + 9x^3 + 6x^2 + 3x
p(2) = 17
```


## 申論及開發報告
### 申論
#### Polynomial 類別
屬性包含：

- termArray：動態陣列儲存多項式項
- capacity：陣列容量
- terms：實際項數

實作方法：

- Add()：多項式加法
- Mult()：多項式乘法
- Eval()：代入求值
- Newterms()：新增項目（並自動擴充記憶體）
#### 多項式加法 Add()
- 依據指數從大到小比對
- 若指數相同，則係數相加
- 若不同，則將較大指數的項直接加入結果多項式中
#### 多項式乘法 Mult()
- 採用雙層迴圈將每一項彼此相乘，並利用加法函式整合重複指數的項：
  ```cpp
  
  for (int i = 0; i < terms; i++) {
    for (int j = 0; j < Poly.terms; j++) {
        newCoef = coef_i * coef_j;
        newExp = exp_i + exp_j;
        Temp.Newterms(newCoef, newExp);
    }
    Res = Res.Add(Temp);
  }
  
#### 多項式求值 Eval(x)
- 將輸入的 x 值代入多項式：

$$P(x) = \sum_i (coef_i \times x^{exp_i})$$


對每一項計算後累加得結果。

### 結論

程式結構清晰，擴充性高，適合作為教學與進階功能開發的基礎。
