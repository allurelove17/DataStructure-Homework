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

### ✅ BST 高度分析：

| n    | Height | log₂(n) | Ratio |
|------|--------|---------|-------|
| 100  | ~11    | 6.64    | ~1.65 |
| 1000 | ~18    | 9.97    | ~1.80 |
| 10000| ~27    | 13.29   | ~2.03 |

- 隨著 n 增加，高度略大於 log₂(n)。
- 原因為隨機插入未保證平衡，存在偏斜的情況。

---

### ✅ 外部排序分析（k-way merge）：

| k  | Input Time (s) | 備註                     |
|----|----------------|--------------------------|
| 2  | 9.640          |                          |
| 4  | 7.320          |                          |
| 8  | 6.160          | 最佳點（記憶體與磁碟平衡） |
| 16 | 6.000          |                          |
| 20 | N/A            | 記憶體不足               |

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
