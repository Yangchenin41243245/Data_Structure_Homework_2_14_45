#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

#define TESTSIZE 10 // size of heap
#define OUTFILE "./out.txt"

class node
{
    public:
    int key;
    node() : key(0) {} // default constructor
    node(int k) : key(k) {}
};

template <class T>
class MinPQ
{
public:
    MinPQ() {}; // constructor
    virtual ~MinPQ() {}; // destructor
    virtual bool isEmpty() const = 0;
    virtual const T& top() const = 0; // return top
    virtual void push(const T&) = 0;  // push item
    virtual void pop() = 0;
    protected:
    vector<T> heap;
    T topnode;
};

class MinHeap : public MinPQ<node>
{
public:
    ~MinHeap() {}; // destructor
    MinHeap() {}; // constructor

    bool isEmpty() const {return this->heap.empty();}

    const node& top() const {
        if (isEmpty()) {
            throw ("Heap is empty");
        }
        return this->heap[0];
    }

    void push(const node& item) //insert new node into heap
    { 
        this->heap.push_back(item);
        heapify(0); //remake heap
    }

    void pop()
    {
        if (isEmpty()) {
            throw ("Heap is empty");
        }
        this->topnode = this->heap[0]; // store top before removing
        this->heap[0] = this->heap.back(); // move last element to root
        this->heap.pop_back(); // remove last element
        if (!this->heap.empty()) {
            heapify(0);
        }
    }

    void heapify(int i)
    {
        int n = this->heap.size();
        for (int i = (n / 2) - 1; i >= 0; --i) {
            adjustMin(i, n);
        }
    }

    void adjustMin(int i,int n)
    {
        int left = i * 2 + 1;
        int right = i * 2 + 2;
        int min = i;

        if (left < this->heap.size() && this->heap[left].key < this->heap[min].key) {
            min = left;
        }
        if (right < this->heap.size() && this->heap[right].key < this->heap[min].key) {
            min = right;
        }
        if (min != i) {
            swap(this->heap[i], this->heap[min]);
            adjustMin(n,min); // recursive heapify
        }
    }


    string print() const //print heap
    {
        string s;
        int counter = 0;
        for (const auto& item : this->heap) {
            cout << item.key << " ";
            s += to_string(item.key) + " ";
            counter++;
            if(!counter%10) s += "\n"; //10 items per row
        }
        return s;
    }
};

int main()
{
    srand(time(0)); // set rand seed
    
    ofstream outFile(OUTFILE);
    if (!outFile) {
        cerr << "Error opening output file." << endl;
        return 1;
    }

    MinHeap minHeap;
    //make sample case
    for (int i = 0; i < TESTSIZE;i++){
        int key = rand() % TESTSIZE; // generate random keys
        minHeap.push(node(key));
    }
    minHeap.heapify(0); // first sorting
    outFile << minHeap.print();
    

    cout << "Top element: " << minHeap.top().key << endl;
    outFile << "Top element: " << minHeap.top().key << endl;

    while (!minHeap.isEmpty()) {
        outFile << "Removing: " << minHeap.top().key << endl;
        minHeap.pop();
    }
    outFile.close();

    return 0;
}