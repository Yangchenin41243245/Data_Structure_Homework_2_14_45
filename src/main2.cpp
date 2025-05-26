#include <iostream>
#include <vector>
#include <fstream>
#include <stack>
using namespace std;

#define TESTSIZE 10 // size of heap
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
    dictionary(){}//start empty
    virtual ~dictionary() {}

    virtual bool isEmpty() const = 0;
    virtual pair<K,E> find(const K&) const = 0; // find item by key
    virtual void insert(const K&, const E&) = 0; 
    virtual void remove(const K&) = 0;
};



template <class K, class E>
class BST : public dictionary<K, E>
{
    private:
    node<K, E>* root; // root of the BST

    public:
    // Define node type for this BST
    using Node = node<K, E>;
    // Constructor to initialize root to nullptr
    BST() : root(nullptr) {}

    // check empty
    bool isEmpty() const {return this->root == nullptr;}

    // search function
    pair<K, E> find(const K& k) const { return find(this->root, k); }
    pair<K, E> find(Node* p, const K& k) const // find node with key k
    {
        if (!p) throw("key not found");
        if (k < p->data.first) return find(p->left, k);
        if (k > p->data.first) return find(p->right, k);
        return p->data; // found pair, return data
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
        if (this->root) // not empty
        {
            if (item.first < pp->data.first) pp->left = p; // link p to parent
            else pp->right = p;
        }
        else
            this->root = p; // tree empty, set p pos as root
    }

    //remove function
    void remove(const K& k) {
     

    }
    //print all nodes
    string printall()
    {
        string out;
        stack<node<K, E>*> s;
        node<K, E>* current = root;
        while (current != nullptr || !s.empty())
        {
            while (current != nullptr)
            {
                s.push(current);
                current = current->left;
            }
            current = s.top();
            s.pop();
            out += to_string(current->data.first) + " : " + to_string(current->data.second) + "\n";
            current = current->right;
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
    BST<int, int> bst;
    bst.insert(make_pair(10, 100));
    bst.insert(make_pair(20, 200));
    bst.insert(make_pair(5, 50));

    fout<<bst.printall();

    if (!bst.isEmpty()) {
        auto item = bst.find(10);
        fout << "Found: " << item.first << ", " << item.second << endl;
    } else {
        fout << "BST is empty." << endl;
    }

    return 0;
}