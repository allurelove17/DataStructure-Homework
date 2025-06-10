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

## 測試與驗證

### 2.(a) BST Height/log2(n) 
| n | Height | log2_n | Height_log2_n_ratio |
|---:|---:|---:|---:|
| 100 | 13 | 6.6439 | 1.9567 |
| 500 | 18 | 8.9658 | 2.0076 |
| 1000 | 23 | 9.9658 | 2.3079 |
| 2000 | 23 | 10.9658 | 2.0974 |
| 3000 | 27 | 11.5507 | 2.3375 |
| 4000 | 24 | 11.9658 | 2.0057 |
| 5000 | 26 | 12.2877 | 2.1159 |
| 6000 | 28 | 12.5507 | 2.2309 |
| 7000 | 32 | 12.7731 | 2.5053 |
| 8000 | 29 | 12.9658 | 2.2367 |
| 9000 | 31 | 13.1357 | 2.36 |
| 10000 | 30 | 13.2877 | 2.2577 |


![BST Height vs log2(n)](https://github.com/allurelove17/DataStructure-Homework/blob/main/homework2/HeightRatio.png)

由上表可知Height/log2(n) ratio都在2左右


### 2.(b) BST 刪除函式時間複雜度 O(log n)

1. Deleting a leaf node: O(log n)
2. Deleting a node with one child: O(log n)
3. Deleting a node with two children: O(log n)

### 3.(a)

### **第一步：算總共要跑幾趟 (P)**

[cite_start]我們有 `m` 堆已排好序的資料 [cite: 2][cite_start]，每次能將 `k` 堆合併成一堆 [cite: 3]。

* 第一趟：`m` 堆變成 `m/k` 堆。
* 第二趟：`m/k` 堆再變成 `m/k²` 堆。
* ...這樣一直下去，直到全部合併成 1 堆為止。

這個過程需要的趟數，用數學來表示就是對數 `log`。因為 `m` 可能不是 `k` 的剛好次方，所以要無條件進位。

$$P = \lceil \log_k m \rceil$$

---

### **第二步：算每一趟要花多久 ($t_{pass}$)**

在每一趟中，我們都得把全部 `n` 筆資料從磁碟讀進來一次。這個時間包含兩部分：真正「傳輸資料」的時間，和來回「跑腿」的磁碟讀取頭等待時間。

1.  **跑腿時間 (磁碟額外開銷)**
    * 我們的記憶體 `S` 要分給 `k` 個輸入和 1 個輸出用，所以總共切成 `k+1` 份，每一份就是一個緩衝區大小 `B = S / (k+1)`。
    * 每一趟總共要讀取 `n / B` 個區塊。
    * 每一次讀取都要花費 `t_s + t_l` 的探頭和延遲時間。
    * **總跑腿時間** = (總區塊數) × (每次跑腿時間) = $\frac{n(k+1)}{S} \times (t_s + t_l)$

2.  **傳輸時間**
    * 這部分很單純，就是把 `n` 筆資料全部傳輸一遍所需的時間。
    * **總傳輸時間** = $n \times t_t$

把這兩部分時間加起來，就是跑一趟要花的總時間：
$$t_{\text{input\_pass}}(k) = \frac{n(k+1)}{S}(t_s + t_l) + n \cdot t_t$$

---

### **第三步：組合最終公式**

最後，把「總趟數」和「每一趟的時間」乘起來，就得到了我們的最終答案。

$$t_{\text{input}}(k) = \lceil \log_k m \rceil \left[ \frac{n(k+1)}{S}(t_s + t_l) + n \cdot t_t \right]$$

### 3.(b)
根據上述推導的公式 $$t_{\text{input}}$$ 勢必存在一個最小值,而題目假設 $$t_{\text{CPU}}$$ 是一個常數,若此數值小於 $$t_{\text{input}}$$ 則題目的假設不成立

![image](https://github.com/allurelove17/DataStructure-Homework/blob/main/homework2/t_input_vs_k_plot.png)

## 申論及開發報告

1. 我們對 BST 插入大量隨機資料後發現，樹高 (Height) 與 $log_2(n)$ 的比值的確如理論預期，穩定地落在 2 左右。這驗證了我們的 BST 程式碼在平均情況下有著接近理想的效能，沒有出現意外的退化。

2. 在分析外部排序時，理解了 `k` 值並非越大越好。我們推導的公式清楚地顯示，雖然增加 `k` 可以減少合併的總趟數，但同時也會讓單趟的 I/O 負擔變重。我們的計算也證實了，因為 I/O 時間存在一個無法突破的最小值，所以「$t_{CPU} \approx t_{input}$」這個條件並非總是成立。
