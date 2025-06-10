#include <iostream>
#include <random>
#include <vector>
#include <cmath>
#include <iomanip>
#include <fstream>

template <class T>
class PQ {
public:
    virtual ~PQ() {}
    virtual bool IsEmpty() const = 0;
    virtual const T& Top() const = 0;
    virtual void Push(const T&) = 0;
    virtual void Pop() = 0;
};

template <class T>
class MinHeap : public PQ<T> {
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

template <class T>
class MaxHeap : public PQ<T> {
private:
    std::vector<T> heap;

    void heapifyUp(int index) {
        if (index == 0) return;
        int parent = (index - 1) / 2;
        if (heap[index] > heap[parent]) {
            std::swap(heap[index], heap[parent]);
            heapifyUp(parent);
        }
    }

    void heapifyDown(int index) {
        int left = 2 * index + 1;
        int right = 2 * index + 2;
        int largest = index;

        if (left < heap.size() && heap[left] < heap[largest]) {
            largest = left;
        }
        if (right < heap.size() && heap[right] < heap[largest]) {
            largest = right;
        }

        if (largest != index) {
            std::swap(heap[index], heap[largest]);
            heapifyDown(largest);
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
        if (node == nullptr) {
            return 0;
        }
        return 1 + std::max(getHeight(node->left), getHeight(node->right));
    }

    TreeNode* findMin(TreeNode* node) {
        while (node->left != nullptr) {
            node = node->left;
        }
        return node;
    }

    TreeNode* deleteNode(TreeNode* node, int key) {
        if (node == nullptr) {
            return node;
        }

        if (key < node->key) {
            node->left = deleteNode(node->left, key);
        }
        else if (key > node->key) {
            node->right = deleteNode(node->right, key);
        }
        else {
            if (node->left == nullptr && node->right == nullptr) {
                delete node;
                return nullptr;
            }

            else if (node->left == nullptr) {
                TreeNode* temp = node->right;
                delete node;
                return temp;
            }
            else if (node->right == nullptr) {
                TreeNode* temp = node->left;
                delete node;
                return temp;
            }

            else {
                TreeNode* temp = findMin(node->right);

                node->key = temp->key;

                node->right = deleteNode(node->right, temp->key);
            }
        }

        return node;
    }

    void destroyTree(TreeNode* node) {
        if (node != nullptr) {
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

    int height() {
        return getHeight(root);
    }

    void deleteKey(int key) {
        root = deleteNode(root, key);
    }

    bool isEmpty() {
        return root == nullptr;
    }
};

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

void analyzeHeightRatio() {
    std::cout << "Part (a): Binary Search Tree Height Analysis\n";
    std::cout << "==========================================\n\n";

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 100000);

    std::vector<int> sizes = { 100, 500, 1000, 2000, 3000, 4000, 5000, 6000, 7000, 8000, 9000, 10000 };

    std::ofstream csvFile("bst_height_analysis.csv");
    csvFile << "n,Height,log2_n,Height_log2_n_ratio\n";

    std::cout << std::setw(8) << "n"
        << std::setw(12) << "Height"
        << std::setw(12) << "log2(n)"
        << std::setw(15) << "Height/log2(n)" << std::endl;
    std::cout << std::string(47, '-') << std::endl;

    for (int n : sizes) {
        BST bst;

        for (int i = 0; i < n; i++) {
            bst.insert(dis(gen));
        }

        int height = bst.height();
        double log2n = std::log2(n);
        double ratio = height / log2n;

        csvFile << n << "," << height << "," << std::fixed << std::setprecision(4) << log2n
            << "," << std::fixed << std::setprecision(4) << ratio << "\n";

        std::cout << std::setw(8) << n
            << std::setw(12) << height
            << std::setw(12) << std::fixed << std::setprecision(2) << log2n
            << std::setw(15) << std::fixed << std::setprecision(3) << ratio << std::endl;
    }

    csvFile.close();
}

void demonstrateDelete() {
    std::cout << "Part (b): Binary Search Tree Delete Function\n";
    std::cout << "==========================================\n\n";

    BST bst;

    std::vector<int> values = { 50, 30, 70, 20, 40, 60, 80, 10, 25, 35, 45 };

    std::cout << "Inserting values: ";
    for (int val : values) {
        std::cout << val << " ";
        bst.insert(val);
    }
    std::cout << "\n";
    std::cout << "Initial tree height: " << bst.height() << "\n\n";

    std::vector<int> toDelete = { 10, 30, 50 };

    for (int key : toDelete) {
        std::cout << "Deleting key: " << key << "\n";
        bst.deleteKey(key);
        std::cout << "Tree height after deletion: " << bst.height() << "\n\n";
    }
}

int main() {
    testMinHeap();
    std::cout << "==========================================\n";
    analyzeHeightRatio();
    std::cout << "==========================================\n";
    demonstrateDelete();

    return 0;
}
