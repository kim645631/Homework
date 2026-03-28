#include <iostream>
#include <string>
using namespace std;

template <class T>
class MinPQ {
public:
    virtual ~MinPQ() {}
    virtual bool IsEmpty() const = 0;
    virtual const T& Top() const = 0;
    virtual void Push(const T& x) = 0;
    virtual void Pop() = 0;
};

template <class T>
class MinHeap : public MinPQ<T> {
private:
    T* heap;
    int heapSize;
    int capacity;

    void ChangeSize1D(int newCapacity) {
        if (newCapacity < 1) newCapacity = 1;

        T* newHeap = new T[newCapacity + 1];
        int copySize = (heapSize < newCapacity) ? heapSize : newCapacity;

        for (int i = 1; i <= copySize; i++) newHeap[i] = heap[i];

        delete[] heap;
        heap = newHeap;
        capacity = newCapacity;

        if (heapSize > capacity) heapSize = capacity;
    }

public:
    explicit MinHeap(int theCapacity = 10)
        : heap(nullptr), heapSize(0), capacity(theCapacity) {
        if (capacity < 1) capacity = 1;
        heap = new T[capacity + 1];
    }

    ~MinHeap() override { delete[] heap; }

    bool IsEmpty() const override { return heapSize == 0; }

    const T& Top() const override {
        if (IsEmpty()) throw string("MinHeap::Top: heap is empty");
        return heap[1];
    }

    void Push(const T& x) override {
        if (heapSize == capacity) ChangeSize1D(capacity * 2);

        int currentNode = ++heapSize;
        while (currentNode != 1 && x < heap[currentNode / 2]) {
            heap[currentNode] = heap[currentNode / 2];
            currentNode /= 2;
        }
        heap[currentNode] = x;
    }

    void Pop() override {
        if (IsEmpty()) throw string("MinHeap::Pop: heap is empty");

        T lastE = heap[heapSize--];

        int currentNode = 1;
        int child = 2;

        while (child <= heapSize) {
            if (child < heapSize && heap[child] > heap[child + 1]) child++;
            if (lastE <= heap[child]) break;

            heap[currentNode] = heap[child];
            currentNode = child;
            child *= 2;
        }
        heap[currentNode] = lastE;
    }

    void PrintArray() const {
        for (int i = 1; i <= heapSize; i++) {
            cout << heap[i];
            if (i != heapSize) cout << " ";
        }
        cout << "\n";
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    MinHeap<int> h((n > 10) ? n : 10);

    for (int i = 0; i < n; i++) {
        int x;
        cin >> x;
        h.Push(x);
    }

    h.PrintArray();

    return 0;
}
