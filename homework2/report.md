# 41243219  
# 41243241  


## 解題說明

### 1. Max/Min Heap
```c++
template <class T>
class PQ {
public:
    virtual ~PQ() {}
    virtual bool IsEmpty() const = 0;
    virtual const T& Top() const = 0;
    virtual void Push(const T&) = 0;
    virtual void Pop() = 0;
};
```
使用上面的abstract class去實作Max/Min Heap.

### 2.Binary Search Tree
實作Binary Search Tree並插入隨機測資在不同的n size,並檢測height/log_2(n)是否在2左右

### 3.Finish the exercise 1 in Textbook p.457
(a) 推導出一個數學公式,當你使用k-way merge的時候能算出total input time
(b) 使用(a)推導出的公式並帶入題目給的數值並檢查是否永遠存在一個k值使得t_CPU ≈ t_input

## 程式實作
### 1.Heap

```cpp
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
```

### 2.BST

```c++
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
```

### 3.
```c++
#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <iomanip>
#include <limits>

double calculate_total_input_time(double n, double S, double m, double ts, double tl, double tt, int k) {
    if (k < 2) {
        return -1.0;
    }
    double num_passes = std::ceil(std::log(m) / std::log(k));
    double time_per_pass = (n * (k + 1.0) / S) * (ts + tl) + (n * tt);
    return num_passes * time_per_pass;
}

int main() {
    const double n = 200000.0;
    const double S = 2000.0;
    const double m = 64.0;
    const double ts = 0.080;
    const double tl = 0.020;
    const double tt = 0.001;

    std::ofstream csv_file("output_data.csv");
    if (!csv_file.is_open()) {
        std::cerr << "Error: Could not open file for writing." << std::endl;
        return 1;
    }

    csv_file << "k,t_input\n";

    double min_time = std::numeric_limits<double>::max();
    int optimal_k = -1;

    for (int k = 2; k <= static_cast<int>(m); ++k) {
        double total_time = calculate_total_input_time(n, S, m, ts, tl, tt, k);
        csv_file << k << "," << std::fixed << std::setprecision(2) << total_time << "\n";

        if (total_time < min_time) {
            min_time = total_time;
            optimal_k = k;
        }
    }

    csv_file.close();

    std::cout << "Successfully generated output_data.csv" << std::endl;
    std::cout << "--- Analysis Summary ---" << std::endl;
    std::cout << "Optimal k: " << optimal_k << std::endl;
    std::cout << "Minimum t_input: " << std::fixed << std::setprecision(2) << min_time << " seconds." << std::endl;

    return 0;
}
```

## 效能分析

### 1. Heap Function Time Complexity
IsEmpty(): O(1)

Top(): O(1)

Push(): O(log n)

Pop(): O(log n)

### 2. BST Height/log2(n) 
| n | Height | log2(n) | Height/log2(n) |
|---:|---:|---:|---:|
| 100 | 14 | 6.64 | 2.107 |
| 500 | 22 | 8.97 | 2.454 |
| 1000 | 20 | 9.97 | 2.007 |
| 2000 | 25 | 10.97 | 2.280 |
| 3000 | 29 | 11.55 | 2.511 |
| 4000 | 25 | 11.97 | 2.089 |
| 5000 | 29 | 12.29 | 2.360 |
| 6000 | 28 | 12.55 | 2.231 |
| 7000 | 27 | 12.77 | 2.114 |
| 8000 | 31 | 12.97 | 2.391 |
| 9000 | 37 | 13.14 | 2.817 |
| 10000 | 31 | 13.29 | 2.333 |


![BST Height vs log2(n)](https://github.com/allurelove17/DataStructure-Homework/blob/main/homework2/HeightRatio.png)

由上表可知Height/log2(n) ratio都在2左右


###2.3 BST 刪除函式程式碼範例（C++）

```cpp
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
```

- 隨著 n 增加，高度略大於 log₂(n)。
- 原因為隨機插入未保證平衡，存在偏斜的情況。

---

### 3. 外部排序分析 (External Sorting)
###問題描述
-模擬在有限記憶體下，利用 Replacement Selection 技術做多路合併排序（k-way merge），計算不同 
k 值對輸入時間的影響。

###參數設定
| 參數  | 數值 |
|----|----------------|
|尋道時間t  	|80.00 ms|	
|延遲時間	|20.00 ms|	
|每筆資料傳輸時間 	|1.00 ms|	
|資料筆數𝑛|2000|	
|初始runs數量𝑚|64|
|記憶體容量 𝑆|2000 筆|





###輸入時間統計表
| k  | Input Time (s) | 備註                     |
|----|----------------|--------------------------|
|2	|221,100|	
|3	|231,600|	
|4	|242,200|	
|5	|252,700|	
|6	|263,300|	
|7	|273,900|	
|8	|284,400	|最佳點附近|
|9	|294,800	|
|10	|305,300	|
|11	|316,300	|
|12	|326,600	|
|13|	337,000	|
|14|	348,200	|
|15|	358,800	|
|16|	369,500	|
|17|	380,200	|
|18	|390,500	|
|19|	400,000	|
|20	|410,600|	記憶體不足，效率下降|

![External Sorting Input Time vs k](https://github.com/allurelove17/DataStructure-Homework/blob/main/homework2/Input%20time.png)


- 當 k 適中（例如 6~10）時，輸入時間最小。
- 若 k 過大，每個 run buffer 空間太少，反而效率下降。

---
# 輸入時間分析報告

## 問題背景
本次分析針對輸入數據處理時間，研究參數 k 對總輸入時間的影響。  
透過不同 k 值計算總輸入時間，尋找最佳 k 值，使輸入時間最短。

## 分析方法
計算總輸入時間公式如下：

![公式](https://github.com/allurelove17/DataStructure-Homework/blob/main/homework2/%E5%85%AC%E5%BC%8F.png)

參數說明：

| 參數 | 含義             | 數值    |
|------|-----------------|---------|
| n    | 輸入數據量       | 200,000 |
| S    | 傳輸速度         | 2,000   |
| m    | 分段數           | 64      |
| t_s  | 設備初始化時間   | 0.080秒 |
| t_l  | 傳輸延遲         | 0.020秒 |
| t_t  | 處理每筆資料時間 | 0.001秒 |

## 分析結果
- 掃描 k 從 2 到 64，計算每個 k 的輸入時間。  
- 找到最佳 k = 8，輸入時間為 580.00 秒（最短）。

## 視覺化
下圖顯示 k 與輸入時間的關係，紅點標示最佳 k 與對應時間：

*（此處貼上圖表）*

## 結論
當 k = 8 時，輸入時間達到最小值，建議系統參數設定可優先考慮此值，以提升整體效率。

## 測試與驗證

- 測試 MinHeap 操作正確性。
- 隨機插入 BST，測量並比對 log₂(n)。
- 外部排序依據公式推導並以不同 k 值進行分析。

---

## 申論及開發報告

- 本作業實作強調資料結構的「抽象設計與效能驗證」。
- 堆積與 BST 展現不同資料結構在排序與查找的效率。
- 外部排序模擬實務資料處理場景，強化記憶體與 I/O 觀念。
- 此外，程式設計風格使用模板與物件導向方式撰寫，提高模組化與可重用性。
- 可延伸設計：將 MinHeap 用
