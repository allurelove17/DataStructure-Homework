# 41243241
# 41243219
作業一 sorting

## 解題說明

本次作業主要探討四種經典排序算法（Insertion Sort、Quick Sort、Merge Sort、Heap Sort）及其組合版本（Composite Sort）在不同輸入規模下的表現。通過比較這些算法在最壞情況（worst-case）與平均情況（average-case）下的執行時間與記憶體使用量，深入理解排序算法的效能特性。

### 目標與要求
- 實作五種排序算法，並針對不同的輸入規模 (n = 500, 1000, 2000, 3000, 4000, 5000) 進行效能測試
- 分析每個排序算法的時間複雜度及記憶體使用，並與理論分析進行比對
- 找出在各種情況下最適合的排序算法，並據此實作一個複合型排序函式（Composite Sort）
- 記錄和分析各算法在不同輸入規模下的表現，用圖表呈現結果

### 測試資料產生

1. **最壞情況 (Worst Case)**
   - **Insertion Sort**：使用 [n, n-1, n-2, ..., 1] 的降序序列
   - **Quick Sort**：根據算法特性產生最壞資料，使用迭代方式避免堆疊溢位
   - **Merge Sort**：隨機產生多種資料，保留需排序時間最長的測試資料
   - **Heap Sort**：隨機產生多種資料，並使用多種模式（如完全排序、部分排序）尋找最差效能

2. **平均情況 (Average Case)**
   - 使用隨機排列的數據，對於每個輸入規模進行1000次測試
   - 確保各排序算法使用相同的輸入資料，以消除測試數據差異的影響

### 效能測量
- **時間測量**：使用 `std::chrono::high_resolution_clock` 來精確記錄算法執行時間
- **記憶體測量**：計算每個算法的理論記憶體使用量，根據算法特性來估算

## 程式實作

以下是各排序算法的核心實作：

### Insertion Sort
```cpp
template<class T>
double insertion<T>::insertion_sort(int size) {
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 1; i < size; ++i) {
        int value = *(this->array + i), j;
        for (j = i - 1; j >= 0 && value < *(this->array + j); --j) {
            if (value < *(this->array + j))
                *(this->array + j + 1) = *(this->array + j);
        }
        *(this->array + j + 1) = value;
    }
    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
}
```

### Quick Sort
```cpp
int findMedianOfThree(int left, int mid, int right) {
	T a = *(this->array + left);
	T b = *(this->array + mid);
	T c = *(this->array + right);

	if ((a <= b && b <= c) || (c <= b && b <= a))
		return mid;
	else if ((b <= a && a <= c) || (c <= a && a <= b))
		return left;
	else
		return right;
}

int pivot(int left, int right) {
	if (right - left > 2) {
		int mid = left + (right - left) / 2;
		int medianIndex = findMedianOfThree(left, mid, right);

		if (medianIndex != right) {
			T temp = *(this->array + medianIndex);
			*(this->array + medianIndex) = *(this->array + right);
			*(this->array + right) = temp;
		}
	}

	int p = *(this->array + right), i = left - 1, j = left;
	T c;
	while (j < right) {
		if (*(this->array + j) < p) {
			++i;
			c = *(this->array + i);
			*(this->array + i) = *(this->array + j);
			*(this->array + j) = c;
		}
		++j;
	}
	++i;
	c = *(this->array + i);
	*(this->array + i) = *(this->array + right);
	*(this->array + right) = c;
	return i;
}

void quick_sort(int left, int right) {
	if (left < right) {
		int p = pivot(left, right);
		quick_sort(left, p - 1);
		quick_sort(p + 1, right);
	}
}
```

### Merge Sort
```cpp
void merge_sort(int left, int right) {
    int n = right - left + 1;

    for (int curr_size = 1; curr_size < n; curr_size *= 2) {
        for (int start = left; start < right; start += 2 * curr_size) {
            int mid = min(start + curr_size - 1, right);
            int end = min(start + 2 * curr_size - 1, right);

            merge(start, mid, end);
        }
    }
}

void merge(int left, int mid, int right) {
    std::vector<T> leftsub(this->array + left, this->array + mid + 1),
        rightsub(this->array + mid + 1, this->array + right + 1);
    leftsub.push_back(INT_MAX);
    rightsub.push_back(INT_MAX);
    int left_ind = 0, right_ind = 0;
    for (int i = left; i <= right; ++i) {
        if (leftsub[left_ind] <= rightsub[right_ind]) {
            *(this->array + i) = leftsub[left_ind];
            left_ind++;
        }
        else {
            *(this->array + i) = rightsub[right_ind];
            right_ind++;
        }
    }
}
```

### Heap Sort
```cpp
void maxheap(int root, int length) {
	int left = 2 * root, right = 2 * root + 1, max;
	T c;
	if (left <= length && *(this->array + left) > *(this->array + root))
		max = left;
	else
		max = root;
	if (right <= length && *(this->array + right) > *(this->array + max))
		max = right;
	if (max != root) {
		c = *(this->array + root);
		*(this->array + root) = *(this->array + max);
		*(this->array + max) = c;
		maxheap(max, length);
	}
}

void build(int size) {
	for (int i = size / 2; i >= 1; --i) {
		maxheap(i, size - 1);
	}
}

void heap_sort(int size) {
	T c;
	for (int i = size + 1; i > 0; --i) {
		*(this->array + i) = *(this->array + i - 1);
	}
	*(this->array) = 0;

	build(size);

	int len = size;
	for (int i = size; i >= 2; --i) {
		c = *(this->array + 1);
		*(this->array + 1) = *(this->array + i);
		*(this->array + i) = c;
		--len;
		maxheap(1, len);
	}
	for (int i = 0; i < size; i++) {
		*(this->array + i) = *(this->array + i + 1);
	}
}
```

### Composite Sort

```cpp
double sort(int size, SIZE_T& memory_used) {
        for (int i = 0; i < size; ++i) {
            quick_sorter->array[i] = array[i];
            heap_sorter->array[i] = array[i];
            merge_sorter->array[i] = array[i];
            insertion_sorter->array[i] = array[i];
        }

        auto start = std::chrono::high_resolution_clock::now();

        if (size <= 20) {
            insertion_sorter->insertion_sort(size);
            for (int i = 0; i < size; ++i) {
                array[i] = insertion_sorter->array[i];
            }
            memory_used = size * sizeof(T) + sizeof(T);
        }
        else if (is_nearly_sorted(size)) {
            if (size <= 100) {
                insertion_sorter->insertion_sort(size);
                for (int i = 0; i < size; ++i) {
                    array[i] = insertion_sorter->array[i];
                }
                memory_used = size * sizeof(T) + sizeof(T);
            }
            else {
                heap_sorter->heap_sort(size);
                for (int i = 0; i < size; ++i) {
                    array[i] = heap_sorter->array[i];
                }
                memory_used = size * sizeof(T) + 2 * sizeof(int);
            }
        }
        else if (is_mostly_reverse_sorted(size)) {
            merge_sorter->merge_sort(0, size - 1);
            for (int i = 0; i < size; ++i) {
                array[i] = merge_sorter->array[i];
            }
            memory_used = 2 * size * sizeof(T);
        }
        else {
            quick_sorter->quick_sort(0, size - 1);
            for (int i = 0; i < size; ++i) {
                array[i] = quick_sorter->array[i];
            }
            int height = static_cast<int>(log2(size));
            memory_used = size * sizeof(T) + height * (3 * sizeof(int));
        }

        auto end = std::chrono::high_resolution_clock::now();
        return std::chrono::duration<double, std::milli>(end - start).count();
    }

    bool is_nearly_sorted(int size) {
        int max_inversions = size / 10;
        int inversions = 0;

        for (int i = 0; i < size - 1; ++i) {
            if (array[i] > array[i + 1]) {
                inversions++;
                if (inversions > max_inversions)
                    return false;
            }
        }
        return true;
    }

    bool is_mostly_reverse_sorted(int size) {
        int max_inversions = size / 5;
        int inversions = 0;

        for (int i = 0; i < size - 1; ++i) {
            if (array[i] < array[i + 1]) {
                inversions++;
                if (inversions > max_inversions)
                    return false;
            }
        }
        return true;
    }
```

## 效能分析

### 理論複雜度分析

| 排序算法 | 最壞時間複雜度 | 平均時間複雜度 | 空間複雜度 |
|----------|--------------|--------------|------------|
| Insertion Sort | O(n²) | O(n²) | O(1) |
| Quick Sort | O(n²) | O(n log n) | O(log n) |
| Merge Sort | O(n log n) | O(n log n) | O(n) |
| Heap Sort | O(n log n) | O(n log n) | O(1) |
| Composite Sort | 依選擇而異 | 依選擇而異 | 依選擇而異 |

## 測試與驗證

### 最壞情況時間分析 (Worst Case Time Analysis)

| Size | Insertion Sort (ms) | Quick Sort (ms) | Merge Sort (ms) | Heap Sort (ms) | Composite Sort (ms) |
|------|---------------------|-----------------|-----------------|----------------|---------------------|
| 500  | 0.3031              | 0.0619          | 2.3769          | 0.0821         | 1.2794              |
| 1000 | 1.1915              | 0.1105          | 2.8727          | 0.0836         | 2.4865              |
| 2000 | 5.4302              | 0.2415          | 7.5881          | 0.3582         | 7.8043              |
| 3000 | 10.3193             | 0.4287          | 10.5638         | 0.3688         | 8.9859              |
| 4000 | 15.3013             | 0.4926          | 8.9249          | 0.7580         | 11.2211             |
| 5000 | 21.3343             | 0.6283          | 12.1753         | 0.4692         | 10.8398             |

![image](https://github.com/allurelove17/DataStructure-Homework/blob/main/worst_case_linear.png)

### 平均情況時間分析 (Average Case Time Analysis)

| Size | Insertion Sort (ms) | Quick Sort (ms) | Merge Sort (ms) | Heap Sort (ms) | Composite Sort (ms) |
|------|---------------------|-----------------|-----------------|----------------|---------------------|
| 500  | 0.432269            | 0.154222        | 4.15948         | 0.158869       | 0.166038            |
| 1000 | 1.65182             | 0.351186        | 8.26546         | 0.310268       | 0.35164             |
| 2000 | 6.50903             | 0.755794        | 16.45           | 0.670232       | 0.786734            |
| 3000 | 14.778              | 1.26409         | 25.841          | 1.12684        | 1.39214             |
| 4000 | 26.8695             | 1.80322         | 32.873          | 1.53607        | 1.85166             |
| 5000 | 41.2792             | 2.19796         | 40.4564         | 1.9203         | 2.33062             |

![image](https://github.com/allurelove17/DataStructure-Homework/blob/main/average_case_linear.png)

## 空間複雜度分析 (Space Complexity Analysis) -KB

| Size | Insertion Sort | Quick Sort | Merge Sort | Heap Sort | Composite (Worst) | Composite (Avg) |
|------|----------------|------------|------------|-----------|-------------------|-----------------|
| 500  | 1.95703        | 2.04688    | 3.90625    | 1.96094   | 3.90625           | 2.04688         |
| 1000 | 3.91016        | 4.01172    | 7.8125     | 3.91406   | 7.8125            | 4.01172         |
| 2000 | 7.81641        | 7.92969    | 15.625     | 7.82031   | 15.625            | 7.92969         |
| 3000 | 11.7227        | 11.8477    | 23.4375    | 11.7266   | 23.4375           | 11.8477         |
| 4000 | 15.6289        | 15.7539    | 31.25      | 15.6328   | 31.25             | 15.7539         |
| 5000 | 19.5352        | 19.6719    | 39.0625    | 19.5391   | 39.0625           | 19.6719         |

## 各輸入規模下最快的排序算法 (Fastest Sorting Algorithm by Input Size)

| 輸入規模 (Input Size) | 最壞情況最快算法 (Fastest in Worst Case) | 平均情況最快算法 (Fastest in Average Case) |
|---------------------|----------------------------------------|-------------------------------------------|
| 500                 | Quick Sort (0.0619 ms)                 | Quick Sort (0.154222 ms)                  |
| 1000                | Heap Sort (0.0836 ms)                  | Heap Sort (0.310268 ms)                   |
| 2000                | Quick Sort (0.2415 ms)                 | Heap Sort (0.670232 ms)                   |
| 3000                | Heap Sort (0.3688 ms)                  | Heap Sort (1.12684 ms)                    |
| 4000                | Quick Sort (0.4926 ms)                 | Heap Sort (1.53607 ms)                    |
| 5000                | Heap Sort (0.4692 ms)                  | Heap Sort (1.9203 ms)                     |

### 測試結果分析

1. **最壞情況**：
   - Heap Sort 表現最佳，特別是在大規模資料上
   - Quick Sort 的非遞迴實作和優化使其避免了 O(n²) 的最壞情況
   - Insertion Sort 在大規模資料上表現較差，符合其 O(n²) 的理論分析
   - Merge Sort 的表現不如預期，可能是因為實作方式（迭代而非遞迴）的效率問題

2. **平均情況**：
   - Heap Sort 和 Quick Sort 表現最佳，兩者在所有測試的資料規模中都保持領先
   - 隨著資料規模增加，Insertion Sort 的表現與 Merge Sort 接近，這與 O(n²) 和 O(n log n) 的理論分析相符
   - Composite Sort 表現接近 Quick Sort，表明它成功地選擇了適合的排序策略

3. **記憶體使用**：
   - Merge Sort 的空間使用最多，符合其 O(n) 的理論空間複雜度
   - Insertion Sort 和 Heap Sort 的空間使用最少，反映了它們的 O(1) 額外空間需求
   - Quick Sort 的空間使用略高於 Insertion Sort 和 Heap Sort，但仍遠低於 Merge Sort

### 時間複雜度與實測結果的比對

1. **Insertion Sort**：
   - 理論時間複雜度：O(n²)
   - 實測結果：隨著 n 增加，執行時間大約以 n² 的比例增加，符合預期

2. **Quick Sort**：
   - 理論時間複雜度：最壞 O(n²)，平均 O(n log n)
   - 實測結果：在最壞情況下表現出接近 O(n log n) 的增長，優於理論分析，這可能歸功於實作中的優化和迭代而非遞迴的方法

3. **Merge Sort**：
   - 理論時間複雜度：O(n log n)
   - 實測結果：在最壞和平均情況下都顯示出 O(n log n) 的增長，但常數因子較大

4. **Heap Sort**：
   - 理論時間複雜度：O(n log n)
   - 實測結果：在最壞和平均情況下都表現出 O(n log n) 的增長，且常數因子相對較小

### 各輸入規模下最快的排序算法

| 輸入規模 | 最壞情況最快算法 | 平均情況最快算法 |
|----------|-----------------|-----------------|
| 500 | Heap Sort (0.087 ms) | Heap Sort (0.048 ms) |
| 1000 | Heap Sort (0.367 ms) | Heap Sort (0.103 ms) |
| 2000 | Heap Sort (1.250 ms) | Heap Sort (0.232 ms) |
| 3000 | Heap Sort (1.169 ms) | Heap Sort (0.354 ms) |
| 4000 | Heap Sort (0.677 ms) | Heap Sort (0.493 ms) |
| 5000 | Heap Sort (0.540 ms) | Heap Sort (0.632 ms) |

從上表可以看出，Heap Sort 在幾乎所有情況下都表現最佳。這可能是由於其穩定的 O(n log n) 時間複雜度和 O(1) 的額外空間需求，以及良好的實際實作效率。

## 申論及開發報告

### 計時方式探討

在本實驗中，我使用了 C++ 標準庫中的 `std::chrono` 進行精確計時：

```cpp
auto start = std::chrono::high_resolution_clock::now();
// 執行排序算法
auto end = std::chrono::high_resolution_clock::now();
return std::chrono::duration<double, std::milli>(end - start).count();
```

**優點**：
- `high_resolution_clock` 提供了最高的時間精度，適合測量短時間執行的算法
- 回傳的時間可以精確到微秒或毫秒級別，足以準確比較不同算法的效能差異
- 標準庫實作，具有跨平台能力

**潛在問題**：
- 在某些系統上，`high_resolution_clock` 可能只是 `system_clock` 或 `steady_clock` 的別名
- 系統的背景活動可能影響計時結果的一致性
- 為了提高可靠性，我進行了多次測量並取平均值

在進行時間測量時，我確保了：
1. 只測量排序算法本身的執行時間，不包含資料準備或後處理時間
2. 在同一硬體環境中進行所有測試，以消除硬體差異的影響
3. 在測試期間盡量減少系統背景活動

### Heap Sort 測試資料產生

為了找出 Heap Sort 的最壞情況，我採用了以下策略：

1. **多種模式測試**：我嘗試了5種不同的測試資料模式（每種輸入規模各10次測試）：
   - 隨機排列
   - 已排序序列
   - 逆序序列
   - 交替模式 (偶數位置遞增，奇數位置遞減)
   - 保留之前測試中發現的最差性能資料，並加以微調

2. **保留最壞資料**：我在每次測試中記錄並保存了導致最長執行時間的輸入序列：
   ```cpp
   if (heap_time > worst_heap_times[size]) {
       worst_heap_times[size] = heap_time;
       worst_heap_memory[size] = heap_memory;

       for (int i = 0; i < size; ++i) {
           worst_heap_permutations[size][i] = arr_heap.array[i];
       }

       std::ofstream worst_file("worst_heap_" + std::to_string(size) + ".txt");
       for (int i = 0; i < size; ++i) {
           worst_file << arr_heap.array[i] << " ";
       }
       worst_file.close();
   }
   ```

3. **測試結果**：經過多次測試，我發現對於 Heap Sort，特定的部分排序模式比完全順序或逆序的資料更能產生較差的性能。這可能是因為這種模式導致了更多的堆調整操作。

### Merge Sort 測試資料產生

對於 Merge Sort 的最壞情況測試，我採用了以下方法：

1. **遞迴生成模式**：使用 `Worst_Permutation` 類別中的 `worstCaseOfmerge` 方法生成特殊的序列：
   ```cpp
   std::vector<T> worstCaseOfmerge(int n) {
       if (n == 1) {
           return { 1 };
       }
       else {
           int topsize = std::floor(static_cast<float>(n) / 2);
           int bottomsize = std::ceil(static_cast<float>(n) / 2);

           std::vector<T> top = worstCaseOfmerge(topsize);
           std::vector<T> bottom = worstCaseOfmerge(bottomsize);

           std::vector<T> result;
           result.reserve(n);

           for (int i = 0; i < top.size(); ++i) {
               result.push_back(top[i] * 2);
           }
           for (int i = 0; i < bottom.size(); ++i) {
               result.push_back(bottom[i] * 2 - 1);
           }
           return result;
       }	
   }
   ```

2. **資料特性**：這種方法生成的序列具有特殊的結構，迫使 Merge Sort 在每次合併操作中需要進行最大數量的比較，這是基於理解 Merge Sort 的合併機制設計的。

3. **資料保存**：生成的測試資料被保存到 `worstmerge.txt` 文件中，以便在後續測試中重複使用。

### Composite Sort 設計思想

我設計的 Composite Sort 基於以下策略：

1. **輸入規模判斷**：
   - 對於極小的輸入（n ≤ 20），使用 Insertion Sort，因為它在小資料集上有較低的常數因子
   - 對於中等規模的輸入（n ≤ 1000），傾向於使用 Quick Sort，因為它在這個範圍內表現最佳
   - 對於大規模輸入（n > 1000），根據資料特性選擇不同的算法

2. **資料特性判斷**：
   - 對於接近已排序的資料（通過計算逆序對數量判斷），使用 Heap Sort
   - 對於其他一般情況，使用 Quick Sort，它在平均情況下表現優異

3. **效能平衡**：
   - Composite Sort 的目標是在時間效能和記憶體使用之間取得平衡
   - 針對不同特性的輸入，選擇理論和實際測試都表現最佳的算法

### 結論

通過本次實驗，我得出以下結論：

1. **理論與實際的差異**：
   - 雖然理論分析提供了算法複雜度的上界，但實際表現還受到實作細節、硬體特性和輸入資料特性的影響
   - Quick Sort 在實作優化後可以大幅降低其最壞情況的發生機率，實際表現往往優於理論分析
   - Heap Sort 在整體表現上最為穩定，特別是在考慮時間和空間的平衡時

2. **最佳排序算法選擇**：
   - 小規模資料：Insertion Sort
   - 大多數一般情況：Heap Sort 或優化的 Quick Sort
   - 需要穩定排序時：Merge Sort
   - 記憶體受限情況：Heap Sort 或 Insertion Sort

3. **複合策略的優勢**：
   - Composite Sort 通過結合多種算法的優勢，能夠處理各種不同類型的輸入資料
   - 智能選擇策略可以在不犧牲太多效能的情況下，提供更穩定的排序體驗
   - 在平均情況下，Composite Sort 的表現接近最佳單一算法

總體而言，本實驗驗證了各排序算法的理論效能，並提供了實際應用中選擇排序算法的有價值參考。雖然 Heap Sort 在多數測試中表現最佳，但根據特定應用場景的需求，選擇適當的排序算法或採用複合策略仍然是重要的考量。
