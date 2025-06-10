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

#### **步驟一：計算合併所需的總趟數 (P)**

在合併階段，我們有 `m` 個已排序的順串 (runs)，並且我們使用 `k`-way merge 的方法

* 每一次合併操作可以將 `k` 個順串合併為 1 個新的、更長的順串。
* 因此，每一趟合併可以讓順串的數量減少為原來的 $1/k$。
* 要將 `m` 個順串合併到剩下最後 1 個，所需要的合併趟數 `P` 是一個對數關係。如果 `m` 不是 `k` 的整數次方，則需要向上取整。
* **公式**：
    $$P = \lceil \log_k m \rceil$$

#### **步驟二：計算每一趟合併的輸入時間 ($t_{\text{input\_pass}}$)**

在每一趟合併中，我們都需要將全部的 `n` 筆記錄從磁碟讀取到記憶體中一次。這一趟的時間由兩部分組成：**總傳輸時間**和**總磁碟額外開銷**（探尋時間 `t_s` + 延遲時間 `t_l`）。

1.  **分配記憶體與計算緩衝區大小 (B)**
    * 問題提到，整個容量為 `S` 的記憶體都可用於 I/O 緩衝區。
    * 為了讓 CPU、輸入、輸出三者可以重疊工作 (overlap)，一個標準的模型是配置 `k` 個輸入緩衝區（對應 `k` 個要合併的順串）和 1 個輸出緩衝區。
    * 因此，總記憶體 `S` 被平分為 `k+1` 個緩衝區。
    * **每個緩衝區的大小 (B)**：
        $$B = \frac{S}{k+1} \quad (\text{單位：筆記錄/緩衝區})$$

2.  **計算單趟讀取的總額外開銷**
    * 額外開銷來自於每次讀取一個新的資料區塊（block）時，磁頭需要移動（探尋 `t_s`）和等待磁盤旋轉（延遲 `t_l`）。
    * **單趟需要讀取的區塊總數**：
        $$\text{總區塊數} = \frac{\text{總記錄數 } n}{\text{每個區塊的記錄數 } B} = \frac{n}{S / (k+1)} = \frac{n(k+1)}{S}$$
    * **總額外開銷時間** = (總區塊數) × (每次讀取的額外開銷)
        $$\text{總開銷} = \frac{n(k+1)}{S} \times (t_s + t_l)$$

3.  **計算單趟讀取的總傳輸時間**
    * 這部分比較單純，就是將全部 `n` 筆記錄從磁碟傳輸到記憶體所需的時間。
    * **總傳輸時間** = (總記錄數) × (每筆記錄的傳輸時間 $t_t$)
        $$\text{總傳輸時間} = n \times t_t$$

4.  **組合單趟輸入時間的公式**
    * 將「總額外開銷」和「總傳輸時間」相加，就得到完成一趟合併所需的總輸入時間。
    * **公式**：
        $$t_{\text{input\_pass}}(k) = \frac{n(k+1)}{S}(t_s + t_l) + n \cdot t_t$$

#### **步驟三：組合最終公式**

最後，將「總趟數」和「每一趟的時間」相乘，即可得到最終的總輸入時間 `t_input`。

* 總輸入時間 ($t_{input}$) = (合併總趟數 P) × (每一趟的輸入時間 $t_{\text{input\_pass}}$)
* **最終完整公式**：
    $$t_{\text{input}}(k) = \lceil \log_k m \rceil \left[ \frac{n(k+1)}{S}(t_s + t_l) + n \cdot t_t \right]$$

### 3.(b)
根據上述推導的公式 $$t_{\text{input}}$$ 勢必存在一個最小值,而題目假設 $$t_{\text{CPU}}$$ 是一個常數,若此數值小於 $$t_{\text{input}}$$ 則題目的假設不成立

![image](https://github.com/allurelove17/DataStructure-Homework/blob/main/homework2/t_input_vs_k_plot.png)

## 申論及開發報告

1. 我們對 BST 插入大量隨機資料後發現，樹高 (Height) 與 $log_2(n)$ 的比值的確如理論預期，穩定地落在 2 左右。這驗證了我們的 BST 程式碼在平均情況下有著接近理想的效能，沒有出現意外的退化。

2. 在分析外部排序時，理解了 `k` 值並非越大越好。我們推導的公式清楚地顯示，雖然增加 `k` 可以減少合併的總趟數，但同時也會讓單趟的 I/O 負擔變重。我們的計算也證實了，因為 I/O 時間存在一個無法突破的最小值，所以「$t_{CPU} \approx t_{input}$」這個條件並非總是成立。
