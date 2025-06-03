#include <iostream>
#include <vector>
#include <cmath>
#include <unordered_set>
#include <cstdlib>
#include <ctime>
#include <fstream>

using namespace std;

class Node {
public:
    int key;
    int value;
    Node* left;
    Node* right;

    Node(int k, int v) : key(k), value(v), left(nullptr), right(nullptr) {}
};

class BST {
private:
    Node* root;
    int numNodes;

    Node* insert(Node* node, int key, int value, bool& inserted) {
        if (!node) {
            inserted = true;
            numNodes++;
            return new Node(key, value);
        }
        if (key < node->key)
            node->left = insert(node->left, key, value, inserted);
        else if (key > node->key)
            node->right = insert(node->right, key, value, inserted);
        else
            inserted = false; // duplicate key
        return node;
    }

    Node* minNode(Node* node) {
        while (node && node->left) node = node->left;
        return node;
    }

    Node* deleteNode(Node* node, int key) {
        if (!node) return nullptr;
        if (key < node->key)
            node->left = deleteNode(node->left, key);
        else if (key > node->key)
            node->right = deleteNode(node->right, key);
        else {
            if (!node->left) {
                Node* temp = node->right;
                delete node;
                numNodes--;
                return temp;
            }
            else if (!node->right) {
                Node* temp = node->left;
                delete node;
                numNodes--;
                return temp;
            }
            else {
                Node* successor = minNode(node->right);
                node->key = successor->key;
                node->value = successor->value;
                node->right = deleteNode(node->right, successor->key);
            }
        }
        return node;
    }

    int height(Node* node) const {
        if (!node) return 0; // 定義空樹高度為 0
        return 1 + max(height(node->left), height(node->right));
    }

    void clear(Node* node) {
        if (!node) return;
        clear(node->left);
        clear(node->right);
        delete node;
    }

public:
    BST() : root(nullptr), numNodes(0) {}
    ~BST() { clear(root); }

    bool insert(int key, int value) {
        bool inserted = false;
        root = insert(root, key, value, inserted);
        return inserted;
    }

    void remove(int key) {
        root = deleteNode(root, key);
    }

    int getHeight() const {
        return height(root);
    }

    int size() const {
        return numNodes;
    }
};

void runExperiment(const vector<int>& ns, const string& filename) {
    ofstream fout(filename);
    if (!fout) {
        cerr << "Failed to open output file." << endl;
        return;
    }

    srand(time(0));
    for (int n : ns) {
        BST bst;
        unordered_set<int> used;
        while (bst.size() < n) {
            int key = rand() % (n * 10);
            int value = rand() % 1000;
            if (used.insert(key).second) {
                bst.insert(key, value);
            }
        }
        int h = bst.getHeight();
        double log2n = log2(n);
        double ratio = h / log2n;

        fout << "n = " << n
             << ", height = " << h
             << ", log2(n) = " << log2n
             << ", height/log2(n) = " << ratio << endl;
    }
    fout.close();
}

int main() {
    vector<int> ns = {100, 500, 1000, 2000, 3000, 10000};
    runExperiment(ns, "D:/work/sort_2/bst_height_report.txt");
    return 0;
}
