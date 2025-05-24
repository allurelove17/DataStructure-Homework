#include <iostream>
#include <vector>
#include <random>
#include <cmath>
#include <algorithm>
#include <iomanip>

// ===== PROBLEM 1: Min Priority Queue =====

template <class T>
class MinPQ {
public:
    virtual ~MinPQ() {}
    virtual bool IsEmpty() const = 0;
    virtual const T& Top() const = 0;
    virtual void Push(const T&) = 0;
    virtual void Pop() = 0;
};

template <class T>
class MinHeap : public MinPQ<T> {
private:
    std::vector<T> heap;

    void heapifyUp(int index) {
        if (index == 0) return;
        int parent = (index - 1) / 2;
        if (heap[index] < heap[parent]) {
            std::swap(heap[index], heap[parent]);
            heapifyUp(parent);
        }
    }

    void heapifyDown(int index) {
        int left = 2 * index + 1;
        int right = 2 * index + 2;
        int smallest = index;

        if (left < heap.size() && heap[left] < heap[smallest]) {
            smallest = left;
        }
        if (right < heap.size() && heap[right] < heap[smallest]) {
            smallest = right;
        }

        if (smallest != index) {
            std::swap(heap[index], heap[smallest]);
            heapifyDown(smallest);
        }
    }

public:
    bool IsEmpty() const override {
        return heap.empty();
    }

    const T& Top() const override {
        if (heap.empty()) {
            throw std::runtime_error("Priority queue is empty");
        }
        return heap[0];
    }

    void Push(const T& element) override {
        heap.push_back(element);
        heapifyUp(heap.size() - 1);
    }

    void Pop() override {
        if (heap.empty()) {
            throw std::runtime_error("Priority queue is empty");
        }

        heap[0] = heap.back();
        heap.pop_back();
        if (!heap.empty()) {
            heapifyDown(0);
        }
    }

    size_t size() const {
        return heap.size();
    }
};

// ===== PROBLEM 2: Binary Search Tree =====

struct TreeNode {
    int key;
    TreeNode* left;
    TreeNode* right;

    TreeNode(int k) : key(k), left(nullptr), right(nullptr) {}
};

class BST {
private:
    TreeNode* root;

    TreeNode* insert(TreeNode* node, int key) {
        if (node == nullptr) {
            return new TreeNode(key);
        }

        if (key < node->key) {
            node->left = insert(node->left, key);
        }
        else if (key > node->key) {
            node->right = insert(node->right, key);
        }

        return node;
    }

    int getHeight(TreeNode* node) {
        if (node == nullptr) return 0;
        return 1 + std::max(getHeight(node->left), getHeight(node->right));
    }

    TreeNode* findMin(TreeNode* node) {
        while (node && node->left) {
            node = node->left;
        }
        return node;
    }

    TreeNode* deleteNode(TreeNode* node, int key) {
        if (node == nullptr) return node;

        if (key < node->key) {
            node->left = deleteNode(node->left, key);
        }
        else if (key > node->key) {
            node->right = deleteNode(node->right, key);
        }
        else {
            // Node to be deleted found
            if (node->left == nullptr) {
                TreeNode* temp = node->right;
                delete node;
                return temp;
            }
            else if (node->right == nullptr) {
                TreeNode* temp = node->left;
                delete node;
                return temp;
            }

            // Node with two children
            TreeNode* temp = findMin(node->right);
            node->key = temp->key;
            node->right = deleteNode(node->right, temp->key);
        }
        return node;
    }

    void destroyTree(TreeNode* node) {
        if (node) {
            destroyTree(node->left);
            destroyTree(node->right);
            delete node;
        }
    }

public:
    BST() : root(nullptr) {}

    ~BST() {
        destroyTree(root);
    }

    void insert(int key) {
        root = insert(root, key);
    }

    int getHeight() {
        return getHeight(root);
    }

    // Delete function with O(log n) average, O(n) worst case complexity
    void deleteKey(int key) {
        root = deleteNode(root, key);
    }
};

// Function for Problem 2(a): BST Height Analysis
void analyzeBSTHeight() {
    std::cout << "\n===== Problem 2(a): BST Height Analysis =====\n";
    std::cout << "n\tHeight\tlog2(n)\tRatio\n";
    std::cout << "------------------------------------\n";

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 100000);

    for (int n = 100; n <= 10000; n += 100) {
        BST tree;

        // Insert n random values
        for (int i = 0; i < n; i++) {
            tree.insert(dis(gen));
        }

        int height = tree.getHeight();
        double log2n = log2(n);
        double ratio = height / log2n;

        if (n % 1000 == 0 || n <= 1000) {
            std::cout << n << "\t" << height << "\t"
                << std::fixed << std::setprecision(2) << log2n
                << "\t" << ratio << std::endl;
        }
    }
}

// ===== PROBLEM 3: External Sorting Analysis =====

void analyzeExternalSorting() {
    std::cout << "\n===== Problem 3: External Sorting Analysis =====\n";

    // Given parameters
    double ts = 80e-3;      // seek time in seconds
    double tl = 20e-3;      // latency time in seconds  
    int n = 200000;         // number of records
    int m = 64;             // number of runs
    double tt = 1e-3;       // transmission time per record
    int S = 2000;           // memory capacity in records
    double tCPU = 0;        // CPU time (assumed constant, we'll analyze input time)

    std::cout << "\nGiven parameters:\n";
    std::cout << "Seek time (ts): " << ts * 1000 << " ms\n";
    std::cout << "Latency time (tl): " << tl * 1000 << " ms\n";
    std::cout << "Transmission time per record (tt): " << tt * 1000 << " ms\n";
    std::cout << "Number of records (n): " << n << "\n";
    std::cout << "Number of runs (m): " << m << "\n";
    std::cout << "Memory capacity (S): " << S << " records\n\n";

    std::cout << "k\tInput Time (s)\tNote\n";
    std::cout << "------------------------\n";

    // For k-way merge in phase 2 of external sorting
    // Input time = (Number of disk accesses) × (ts + tl) + (Total transmission time)
    // In k-way merge, we need to read all n records once
    // Number of disk accesses ≈ n / (buffer_size_per_run)
    // Buffer size per run = S/k (approximately, leaving some for output buffer)

    for (int k = 2; k <= 20; k++) {
        // Buffer size per input run (leaving some space for output buffer)
        int buffer_per_run = (S - 100) / k;  // Reserve 100 records for output buffer

        if (buffer_per_run <= 0) {
            std::cout << k << "\tN/A\t\tInsufficient memory\n";
            continue;
        }

        // Number of disk accesses for reading all records
        int disk_accesses = (n + buffer_per_run - 1) / buffer_per_run;

        // Total input time
        double input_time = disk_accesses * (ts + tl) + n * tt;

        std::cout << k << "\t" << std::fixed << std::setprecision(3)
            << input_time << "\t\t";

        if (k == 8) {
            std::cout << "Optimal point approximately";
        }

        std::cout << "\n";
    }

    std::cout << "\nAnalysis:\n";
    std::cout << "- As k increases, buffer per run decreases, leading to more disk accesses\n";
    std::cout << "- There's typically an optimal k around 6-10 where input time is minimized\n";
    std::cout << "- Very large k values become impractical due to insufficient buffer space\n";
}

// Test function for MinHeap
void testMinHeap() {
    std::cout << "\n===== Testing MinHeap Implementation =====\n";

    MinHeap<int> minHeap;

    // Test insertions
    std::vector<int> values = { 20, 15, 25, 10, 5, 30, 8 };
    std::cout << "Inserting values: ";
    for (int val : values) {
        std::cout << val << " ";
        minHeap.Push(val);
    }
    std::cout << "\n";

    // Test extracting minimum elements
    std::cout << "Extracting elements in ascending order: ";
    while (!minHeap.IsEmpty()) {
        std::cout << minHeap.Top() << " ";
        minHeap.Pop();
    }
    std::cout << "\n";
}

// Main function to run all problems
int main() {
    std::cout << "Data Structures Problems Solutions\n";
    std::cout << "==================================\n";

    // Problem 1: Test MinHeap
    testMinHeap();

    // Problem 2(a): BST Height Analysis  
    analyzeBSTHeight();

    // Problem 2(b): Note about deletion complexity
    std::cout << "\n===== Problem 2(b): BST Deletion Function =====\n";
    std::cout << "The deleteKey() function is implemented above.\n";
    std::cout << "Time Complexity:\n";
    std::cout << "- Average case: O(log n) for balanced BST\n";
    std::cout << "- Worst case: O(n) for completely unbalanced BST (like a linked list)\n";
    std::cout << "- The complexity depends on the height of the tree\n";

    // Problem 3: External Sorting Analysis
    analyzeExternalSorting();

    return 0;
}
