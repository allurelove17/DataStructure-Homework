# 41243241
# 41243219
作業一 sorting
## 解題說明
探討四種排序演算法（Insertion Sort、Quick Sort、Merge Sort、Heap Sort）在不同輸入規模下的表現。通過比較這些演算法在最壞情況（worst-case）與平均情況（average-case）下的執行時間與記憶體使用量，深入理解排序算法的效能特性。
並根據分析出的結果寫出在worst-case的情況下能跑出良好 time-complexity的 Composite Sort
### 目標與要求
- 實作五種排序算法，並針對不同的輸入規模 (n = 500, 1000, 2000, 3000, 4000, 5000) 進行效能測試
- 分析每個排序算法的時間複雜度及記憶體使用，並與理論分析進行比對
- 找出在各種情況下最適合的排序算法，並據此實作一個Composite Sort
- 記錄和分析各算法在不同輸入規模下的表現，用圖表及表格呈現結果
### 測試資料產生
1. **最壞情況 (Worst Case)**
- Insertion Sort：使用 [n, n-1, n-2, ..., 1] 的降序序列
- Merge Sort：根據算法特性產生最壞資料，使用迭代方式避免堆疊溢位
- Quick Sort：隨機產生多種資料，保留需排序時間最長的測試資料
- Heap Sort：隨機產生多種資料，保留需排序時間最長的測試資料
2. **平均情況 (Average Case)**
- 使用隨機排列的數據，對於每個輸入規模進行1000次測試
- 確保各排序算法使用相同的輸入資料，以消除測試數據差異的影響
## 程式實作
以下是各排序演算法的核心程式碼：

**Timer**

std::chrono::high_resolution_clock // C++當前提供的Timer中精度最高的

std::chrono::high_resolution_clock::now(); // 可取得程式碼執行到這行當下的時間

std::chrono::duration_cast<std::chrono::microseconds> // 將時間單位做轉換, 這裡轉成microseconds

取得程式執行排序演算法的起始時間和程式跑完排序演算法的結束時間相減後即是執行時間.

精度取microseconds. 圖表將精度轉為milliseconds, 因為"μ"這個符號會造成python script無法順利讀取csv file的資料
```c++
double mergetime(int left, int right, SIZE_T& merge_memory) {
	auto start = std::chrono::high_resolution_clock::now();
	merge_sort(left, right);
	auto end = std::chrono::high_resolution_clock::now();
	return std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
}
```

**Insertion Sort**
```c++
double insertion_sort(int size) {
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

**Quick Sort (Median of Three Method)**
### Median of Three Method可避免Worst Case of Quick Sort
```c++
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

void quick_sort(int left, int right, SIZE_T& quick_memory, int depth, int& max_depth) {
	if (depth > max_depth) {
		max_depth = depth;
	}
	
	if (left < right) {
		int p = pivot(left, right);
		quick_sort(left, p - 1, quick_memory, depth + 1, max_depth);
		quick_sort(p + 1, right, quick_memory, depth + 1, max_depth);
	}
}
```

**Merge Sort (Iterative)**
```c++
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
    for (int i = left; i <= right; i++) {
        temp_array[i] = array[i];
    }

    int i = left;
    int j = mid + 1;
    int k = left;

    while (i <= mid && j <= right) {
        if (temp_array[i] <= temp_array[j]) {
            array[k++] = temp_array[i++];
        }
        else {
            array[k++] = temp_array[j++];
        }
    }

    while (i <= mid) {
        array[k++] = temp_array[i++];
    }

    while (j <= right) {
        array[k++] = temp_array[j++];
    }
}
```

**Heap Sort**
```c++
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

double heap_sort(int size) {
	auto start = std::chrono::high_resolution_clock::now();
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
	auto end = std::chrono::high_resolution_clock::now();
	return std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
}
```

**Random Permutation for Heap Sort & Quick Sort**

std::mt19937 : A Mersenne Twister pseudo-random generator.

std::random_device : 用來產生不確定性的隨機數

std::shuffle(begin, last, g);  將[first,last)內的元素根據g(uniform random number generator)進行重新排序
```c++
std::vector<int> permutation(int n) {
    std::vector<int> result(n);
    for (int i = 0; i < n; i++) {
        result[i] = i + 1;
    }

    std::random_device rd;
    std::mt19937 g(rd());

    std::shuffle(result.begin(), result.end(), g);

    return result;
}
```

```c++
    double worst_quick_time = 0.0, worst_heap_time = 0.0;
    SIZE_T final_quick_memory = 0, final_heap_memory = 0;
    for (int runtime = 0; runtime < 100; ++runtime) { // Run it 100 times and choose the worst one as the worst case.
        std::vector<int> worst = permutation(size);
        
        quick<int> arr_quick(size); // Quick Sort
        SIZE_T quick_memory = 0;

        for (int i = 0; i < size; ++i) { // Random data for Quicksort
            arr_quick.array[i] = worst[i];
        }
        double quick_time = arr_quick.quicktime(0, size - 1, quick_memory);

        if (quick_time > worst_quick_time) { // If find the worse one. Replace the worst quick sort runtime.
            worst_quick_time = quick_time;
            final_quick_memory = quick_memory;
            for (int i = 0; i < size; ++i) {
                worst_quick_permutations[size][i] = worst[i];
            }
        }

        //==============================================================

        heap<int> arr_heap(size); // Heap Sort
        getMemoryUsage(beforeMemory); 
        for (int i = 0; i < size; ++i) {
            arr_heap.array[i] = worst[i];
        }
        double heap_time = arr_heap.heap_sort(size);
        getMemoryUsage(afterMemory);
        SIZE_T heap_memory = afterMemory - beforeMemory;

        if (heap_time > worst_heap_time) { // If find the worse one. Replace the worst heap sort runtime.
            worst_heap_time = heap_time;
            final_heap_memory = heap_memory;
            for (int i = 0; i < size; ++i) {
                worst_heap_permutations[size][i] = worst[i];
            }
        }
    }
    // Worst Case for Quick & Heap
    worst_quick_times[size] = worst_quick_time;
    worst_quick_memory[size] = final_quick_memory;
    worst_heap_times[size] = worst_heap_time;
    worst_heap_memory[size] = final_heap_memory;
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

1. **Insertion Sort**
$$\sum_{i=1}^{n} i = 1 + 2 + 3 + \ldots + n = $$
$$\mathcal{O}\left(\sum_{i=1}^{n} i\right) = \mathcal{O}\left(\frac{(n+1)n}{2}\right) = \mathcal{O}\left(\frac{1}{2}n^2 + \frac{1}{2}n\right) = \mathcal{O}(n^2)$$
- Worst Time Complexity : $$\mathcal{O}\left(\frac{1}{2}n^2 + \frac{1}{2}n\right) = \mathcal{O}(n^2)$$
- Worst Space Complexity : $$\mathcal{O}(1)$$
- Average Time Complexity : $$\mathcal{O}(n^2)$$ Same as Worst Time Complexity
- Average Space Complexity : $$\mathcal{O}(1)$$
2. **Quick Sort(Median-of-three method)**
- Worst Time Complexity : $$\mathcal{O}(n\log n)$$ median-of-three method 會避免出現Worst Case 所以是 $$\mathcal{O}(n\log n)$$
- Worst Space Complexity : $$O(\log n)$$  median-of-three method to choose pivot 會讓遞迴的深度為 $$O(\log n)$$
- Average Time Complexity : $$\mathcal{O}(n\log n)$$ 
- Average Space Complexity : $$O(\log n)$$
3. **Merge Sort**
- Worst Time Complexity : $$\mathcal{O}(n\log n)$$ Iterative層數 $$\mathcal{\log O}(n)$$ , 每層 $$\mathcal{O}(n)$$
- Worst Space Complexity : $$\mathcal{O}(n)$$ 已排序array的大小,總共為n
- Average Time Complexity : $$\mathcal{O}(n\log n)$$
- Average Space Complexity : $$\mathcal{O}(n)$$
4. **Heap Sort**
- Worst Time Complexity : $$\mathcal{O}(n\log n)$$ maxheap $$\mathcal{O}(\log n)$$ build $$\mathcal{O}(n)$$
- Worst Space Complexity : $$\mathcal{O}(1)$$ 不需要額外儲存陣列的資料(In-place algorithm)
- Average Time Complexity : $$\mathcal{O}(n\log n)$$ Same as Worst Time Complexity
- Average Space Complexity : $$\mathcal{O}(1)$$ 不需要額外儲存陣列的資料(In-place algorithm)
## 測試與驗證
## Sorting Algorithm Performance - Average Case

**Average Case - Time Measurements (ms)**

| Size | Insertion_Time | Quick_Time | Merge_Time | Heap_Time | Composite_Time |
| --- | --- | --- | --- | --- | --- |
| 500 | 0.148886 | 0.03787 | 0.045996 | 0.059832 | 0.00004 |
| 1000 | 0.569704 | 0.07998 | 0.098728 | 0.130326 | 0.00009 |
| 2000 | 2.22698 | 0.177848 | 0.219547 | 0.29468 | 0.00019 |
| 3000 | 5.02804 | 0.284464 | 0.354965 | 0.465641 | 0.0003 |
| 4000 | 9.1213 | 0.399969 | 0.506531 | 0.669826 | 0.00043 |
| 5000 | 14.1775 | 0.490993 | 0.629315 | 0.81951 | 0.00053 |

![image](https://github.com/allurelove17/DataStructure-Homework/blob/main/homework1/average_time_complexity_chart.png)

**Average Case - Memory Usage (KB)**

| Size | Insertion_Memory | Quick_Memory | Merge_Memory | Heap_Memory | Composite_Memory |
| --- | --- | --- | --- | --- | --- |
| 500 | 0 | 0.220703 | 1.95312 | 0 | 0 |
| 1000 | 0 | 0.251953 | 3.90625 | 0 | 0 |
| 2000 | 0 | 0.283203 | 7.8125 | 0 | 0 |
| 3000 | 0 | 0.303711 | 11.7188 | 0 | 0 |
| 4000 | 0 | 0.31543 | 15.625 | 0 | 0 |
| 5000 | 0 | 0.326172 | 19.5312 | 0 | 0 |

![image](https://github.com/allurelove17/DataStructure-Homework/blob/main/homework1/average_space_complexity_chart.png)

## Sorting Algorithm Performance - Worst Case

**Worst Case - Time Measurements (ms)**

| Size | Insertion_Time | Quick_Time | Merge_Time | Heap_Time | Composite_Time |
| --- | --- | --- | --- | --- | --- |
| 500 | 0.215 | 0.181 | 0.01698 | 0.062 | 0.0015479 |
| 1000 | 0.772 | 0.166 | 0.03901 | 0.203 | 0.000198 |
| 2000 | 4.158 | 0.309 | 0.09277 | 0.443 | 0.0002578 |
| 3000 | 7.565 | 0.593 | 0.1572 | 0.975 | 0.0007474 |
| 4000 | 12.597 | 0.718 | 0.19946 | 1.235 | 0.0005692 |
| 5000 | 19.432 | 1.033 | 0.2502 | 1.484 | 0.0013376 |

![image](https://github.com/allurelove17/DataStructure-Homework/blob/main/homework1/worst_time_complexity_chart.png)

**Worst Case - Memory Usage (KB)**

| Size | Insertion_Memory | Quick_Memory | Merge_Memory | Heap_Memory | Composite_Memory |
| --- | --- | --- | --- | --- | --- |
| 500 | 0 | 0.203125 | 1.95312 | 0 | 0 |
| 1000 | 0 | 0.25 | 3.90625 | 0 | 0 |
| 2000 | 0 | 0.28125 | 7.8125 | 0 | 0 |
| 3000 | 0 | 0.3125 | 11.7188 | 0 | 0 |
| 4000 | 0 | 0.3125 | 15.625 | 0 | 0 |
| 5000 | 0 | 0.3125 | 19.5312 | 0 | 0 |

![image](https://github.com/allurelove17/DataStructure-Homework/blob/main/homework1/worst_space_complexity_chart.png)

### 結論
根據line chart及table的數據可大致看出實際測試後的複雜度和理論上的複雜度是相符合的

## 申論及開發報告
1. ratio <==> duration
2. !!! merge sort worst case runtime is better than average one !!!
3. #include <Windows.h> // GetCurrentProcess
#include <psapi.h>
4. Merge Sort 使用O(n)的空間會跑比較快
## 參考資料 // Option
