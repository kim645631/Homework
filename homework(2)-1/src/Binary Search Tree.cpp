#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <iostream>
using namespace std;

struct Node {
    int key;
    Node* left;
    Node* right;
    explicit Node(int k) : key(k), left(NULL), right(NULL) {}
};

class BST {
public:
    BST() : root(NULL) {}
    ~BST() { clear(root); }

    void insert(int key) { root = insertRec(root, key); }
    void erase(int key) { root = deleteRec(root, key); }
    int height() const { return heightRec(root); }

private:
    Node* root;

    static Node* insertRec(Node* node, int key) {
        if (node == NULL) return new Node(key);
        if (key < node->key) node->left = insertRec(node->left, key);
        else if (key > node->key) node->right = insertRec(node->right, key);
        return node;
    }

    static int heightRec(Node* node) {
        if (node == NULL) return 0;
        int hl = heightRec(node->left);
        int hr = heightRec(node->right);
        return 1 + (hl > hr ? hl : hr);
    }

    static Node* findMin(Node* node) {
        while (node && node->left) node = node->left;
        return node;
    }

    static Node* deleteRec(Node* node, int key) {
        if (node == NULL) return NULL;

        if (key < node->key) node->left = deleteRec(node->left, key);
        else if (key > node->key) node->right = deleteRec(node->right, key);
        else {
            if (node->left == NULL && node->right == NULL) {
                delete node;
                return NULL;
            }
            else if (node->left == NULL) {
                Node* r = node->right;
                delete node;
                return r;
            }
            else if (node->right == NULL) {
                Node* l = node->left;
                delete node;
                return l;
            }
            else {
                Node* succ = findMin(node->right);
                node->key = succ->key;
                node->right = deleteRec(node->right, succ->key);
            }
        }
        return node;
    }

    static void clear(Node* node) {
        if (!node) return;
        clear(node->left);
        clear(node->right);
        delete node;
    }
};

static int uniformKey(int KEY_MAX) {
    unsigned int a = (unsigned int)rand();
    unsigned int b = (unsigned int)rand();
    unsigned int x = (a << 16) ^ b;
    return (int)(x % (unsigned int)KEY_MAX) + 1;
}

int main() {
    srand(123456);
    const int TRIALS = 50;
    const int KEY_MAX = 1000000000;

    int ns[] = { 100, 500, 1000, 2000, 3000, 4000, 5000, 6000, 7000, 8000, 9000, 10000 };
    int m = (int)(sizeof(ns) / sizeof(ns[0]));

    cout << "n,avgH,avgH/log2n\n";

    for (int idx = 0; idx < m; idx++) {
        int n = ns[idx];
        double sumH = 0.0;

        for (int t = 0; t < TRIALS; t++) {
            BST tree;
            for (int i = 0; i < n; i++) tree.insert(uniformKey(KEY_MAX));
            sumH += tree.height();
        }

        double avgH = sumH / TRIALS;
        double log2n = log((double)n) / log(2.0);
        double ratio = avgH / log2n;

        printf("%d,%.6f,%.6f\n", n, avgH, ratio);
    }
    return 0;
}
