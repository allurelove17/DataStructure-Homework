# 41243219  
# 41243241  


## 1. MinHeap 實作與測試

### 1.1 MinHeap 概述

MinHeap 是一種完全二元樹，滿足父節點值小於等於子節點的特性。主要操作包含：

- `Push`：插入元素並向上調整（up-heapify）  
- `Pop`：取出最小元素並向下調整（down-heapify）  
- `Top`：查看堆頂元素  

### 1.2 程式碼範例（C++ 節錄）

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
---

### ✅ MinHeap 測試結果：

-Inserting values: 20 15 25 10 5 30 8
-Extracting elements in ascending order: 5 8 10 15 20 25 30

- 證明堆結構維持正確，輸出為遞增順序。

---
## 2. 二元搜尋樹 (BST) 高度分析與刪除

### 2.1 BST 概述

BST 是一種結構化的樹，左子樹所有節點值小於根節點，右子樹所有節點值大於根節點。主要操作：

- 插入 (Insert）  
- 刪除 (Delete) 
- 計算高度 (Height)
  
### ✅ BST 高度分析：

| n    | Height | log₂(n) | Ratio |
|------|--------|---------|-------|
|100	|13|	6.64|	1.96|
|200	|18	|7.64	|2.35|
|300	|17	|8.23	|2.07|
|400|	17|	8.64|	1.97|
|500|	19	|8.97|	2.12|
|600	|18	|9.23|	1.95|
|700	|19	|9.45	|2.01|
|800	|22	|9.64	|2.28|
|900	|21	|9.81	|2.14|
|1000	|22	|9.97	|2.21|
|2000	|25	|10.97	|2.28|
|3000	|24	|11.55	|2.08|
|4000	|28	|11.97	|2.34|
|5000	|30	|12.29	|2.44|
|6000	|30	|12.55	|2.39|
|7000	|32	|12.77	|2.51|
|8000	|31	|12.97	|2.39|
|9000	|32	|13.14	|2.44|
|10000	|31	|13.29	|2.33|

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

- 當 k 適中（例如 6~10）時，輸入時間最小。
- 若 k 過大，每個 run buffer 空間太少，反而效率下降。

---

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
