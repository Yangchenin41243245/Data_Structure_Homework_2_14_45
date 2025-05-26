#include <iostream>
#include <vector>
#include <fstream>
#include <stack>
#include <stdexcept>
using namespace std;

#define TESTSIZE 100 // size of heap
#define OUTFILE "./out2.txt"


template <class K,class E>
class node
{
public:
    pair<K, E> data;
    node* left;
    node* right;

    node(const K& k, const E& v) : data(make_pair(k, v)), left(nullptr), right(nullptr)
    {
    }
};

template <class K,class E>
class dictionary
{
public:
struct searchResult
    {
        bool found = false; // true if found
        node<K, E> target; //direct copy target node
    };
    dictionary(){}//start empty
    virtual ~dictionary() {}

    virtual bool isEmpty() const = 0;
    virtual pair<K,E>* find(const K&) const = 0; // find item by key
    virtual void insert(const K&, const E&) = 0; 
    virtual void remove(const K&) = 0;

    
};



template <class K, class E>
class BST : public dictionary<K, E>
{
    private:
    node<K, E>* root; // root of the BST
    int numNodes; //nodes in this tree

public:
    // Define node type for this BST
    using Node = node<K, E>;
    // Constructor to initialize root to nullptr
    BST() : root(nullptr),numNodes(0) {}

    // check empty
    bool isEmpty() const {return this->root == nullptr;}

    // size function
    int size() {
        return numNodes;
    }

    // search function
    pair<K,E >*find(const K& k) const { return find(this->root, k); }
    pair<K,E>*find(Node* p, const K& k) const // find node p with key k
    {
        if (!p)  //can't search anymore 
            return nullptr;
            if (k < p->data.first)
                return find(p->left, k);
            if (k > p->data.first)
                return find(p->right, k);
            return &(p->data);
    }

    // insert function
    void insert(const K& key, const E& value){
        insert(make_pair(key, value)); //insert as whole pair
    }

    void insert(const pair<K, E>& item) {
        Node* p = this->root, *pp = nullptr; // pp is parent of p
        // look for spot
        while (p) {
            pp = p; //this pos as parent
            if (item.first < p->data.first) p = p->left; //compare & go down
            else if (item.first > p->data.first) p = p->right;
            else // same key, update second
            {
                p->data.second = item.second;
                return;
            }
        } //no more p, pp is last parent
        // insert new node
        p = new Node(item.first, item.second); //make new node out of item data
        numNodes++; // increase size of tree
        if (this->root) // not empty
        {
            if (item.first < pp->data.first) pp->left = p; // link p to parent
            else pp->right = p;
        }
        else
            this->root = p; // tree empty, set p pos as root
    }

    //remove function
    void remove(const K& k) 
    {
        node<K, E>* parent = nullptr;
        node<K, E>* current = this->root;
        // find the node to remove and its parent
        while (current && current->data.first != k) {
            parent = current;
            if (k < current->data.first) {
                current = current->left;
            } else {
                current = current->right;
            }
        }
        if (!current) throw runtime_error("Key not found"); // if not found, throw error
        // Case 1: Node to be deleted has no children (leaf node)
        if (!current->left && !current->right) {
            if (current == this->root) {
                this->root = nullptr; // if root is the only node
            } else if (parent->left == current) {
                parent->left = nullptr; // unlink from parent
            } else {
                parent->right = nullptr;
            }
        }
        // Case 2: Node to be deleted has one child
        else if (!current->left || !current->right) {
            node<K, E>* child = current->left ? current->left : current->right; // get the child
            if (current == this->root) {
                this->root = child; // if root, set child as new root
            } else if (parent->left == current) {
                parent->left = child; // link child to parent
            } else {
                parent->right = child;
            }
        }
        // Case 3: Node to be deleted has two children
        else {
            // Find the in-order successor (smallest in the right subtree)
            node<K, E>* succPP = current; //parent of successor
            node<K, E>* successor = current->right;
            while (successor->left) {
                succPP = successor;
                successor = successor->left;
            }
            // Copy the successor's data to the current node
            current->data = successor->data;
            // Remove the successor
            if (succPP->left == successor) {
                succPP->left = successor->right; // unlink successor
            } else {
                succPP->right = successor->right;
            }
        }
        delete current; // free memory
        numNodes--; // decrease size of tree
    }
    //print all nodes
    string printall() //with DFS
    {
        string out;
        out += "BST size: " + to_string(this->size()) + "\n";
        stack<node<K, E>*> s;
        node<K, E>* current = root;
        while (current != nullptr || !s.empty())
        {
            while (current != nullptr)
            {
                s.push(current);
                current = current->left;
            }
            if (!s.empty())
            {
                current = s.top();
                s.pop();
                out += to_string(current->data.first) + " : " + to_string(current->data.second) + "\n";
                current = current->right;
            }
        }
        return out;
    }
    
    private:

};

int main()
{
    //set file paths
    ofstream fout(OUTFILE);
    if (!fout) {
        cerr << "Error opening output file." << endl;
        return 1;
    }

    //make test case
    srand(time(0));
    BST<int, int> bst;
    for (int i = 0;i<TESTSIZE;i++)
    {
        int key = rand() % 100; // random key
        int value = rand() % 1000; // random value
        bst.insert(make_pair(key,value));
    
    }

    fout<<bst.printall();

    if (!bst.isEmpty() && bst.size()>3) 
    {
        for (int i = 0; i < 3;i++)
        {
            int k = rand() % 100; // random key to search
            fout << "Searching for key: " << k << endl;
            auto item = bst.find(k);
            if (item != nullptr) {
                fout << "Found item: " << item->first << " : " << item->second << endl;
            } else {
                fout << "Key not found: " << k << endl;
            }
        }
    } 

    return 0;
}