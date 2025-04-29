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
- Merge Sort：根據演算法特性使用recursive推出測試資料
- Quick Sort：隨機產生多種資料，保留需排序時間最長的測試資料
- Heap Sort：隨機產生多種資料，保留需排序時間最長的測試資料
2. **平均情況 (Average Case)**
- 使用隨機排列的數據，對於每個輸入規模進行1000次測試
- 確保各排序算法使用相同的輸入資料，以消除測試數據差異的影響
## 程式實作
以下是核心程式碼：

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
## Composite Sorting Algorithm Implementation
```c++
    double sort(int size, SIZE_T& memory_used) {
        auto start = std::chrono::high_resolution_clock::now();

        if (size <= 75) {
            for (int i = 0; i < size; ++i) {
                insertion_sorter->array[i] = array[i];
            }
            insertion_sorter->insertion_sort(size);
            for (int i = 0; i < size; ++i) {
                array[i] = insertion_sorter->array[i];
            }
            memory_used = size * sizeof(T) + sizeof(T);
        }
        else {
            for (int i = 0; i < size; ++i) {
                quick_sorter->array[i] = array[i];
            }
            SIZE_T quick_memory = 0;
            quick_sorter->quicktime(0, size - 1, quick_memory);
            for (int i = 0; i < size; ++i) {
                array[i] = quick_sorter->array[i];
            }
            memory_used = quick_memory;
        }

        auto end = std::chrono::high_resolution_clock::now();
        return std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
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

| Size | Insertion_Time | Quick_Time | Merge_Time | Heap_Time |
| --- | --- | --- | --- | --- |
| 500 | 0.11187 | 0.028976 | 0.034623 | 0.045007 |
| 1000 | 0.435333 | 0.061614 | 0.075538 | 0.099278 |
| 2000 | 1.70098 | 0.13475 | 0.16538 | 0.219291 |
| 3000 | 3.76532 | 0.212912 | 0.25983 | 0.34493 |
| 4000 | 6.61118 | 0.288726 | 0.354981 | 0.474089 |
| 5000 | 10.3408 | 0.374157 | 0.458688 | 0.606962 |

![image](https://github.com/allurelove17/DataStructure-Homework/blob/main/homework1/average_time_complexity_chart.png)

**Average Case - Memory Usage (KB)**

Insertion & Heap & Composite Memory: O(1)

Quick Memory: O(logn)
- log₂(5000)/log₂(500) ≈ 12.29/8.97 ≈ 1.37~
- 0.327148/0.219727 ≈ 1.488~

1.37 跟 1.478很相近
  
Merge Memory: O(n)
- 5000/500 = 10
- 19.5312/1.95312 = 10

| Size | Insertion_Memory | Quick_Memory | Merge_Memory | Heap_Memory |
| --- | --- | --- | --- | --- |
| 500 | 0 | 0.219727 | 1.95312 | 0 |
| 1000 | 0 | 0.251953 | 3.90625 | 0 |
| 2000 | 0 | 0.283203 | 7.8125 | 0 |
| 3000 | 0 | 0.302734 | 11.7188 | 0 |
| 4000 | 0 | 0.317383 | 15.625 | 0 |
| 5000 | 0 | 0.327148 | 19.5312 | 0 |

![image](https://github.com/allurelove17/DataStructure-Homework/blob/main/homework1/average_space_complexity_chart.png)

## Sorting Algorithm Performance - Worst Case

**Worst Case - Time Measurements (ms)**

| Size | Insertion_Time | Quick_Time | Merge_Time | Heap_Time |
| --- | --- | --- | --- | --- |
| 500 | 0.328 | 0.231 | 0.02341 | 0.251 |
| 1000 | 0.83 | 0.084 | 0.04255 | 0.134 |
| 2000 | 3.127 | 1.25 | 0.08999 | 0.63 |
| 3000 | 7.331 | 0.574 | 0.14357 | 0.464 |
| 4000 | 12.295 | 0.443 | 0.21391 | 1.031 |
| 5000 | 20.095 | 0.604 | 0.24903 | 1.141 |

![image](https://github.com/allurelove17/DataStructure-Homework/blob/main/homework1/worst_time_complexity_chart.png)

**Worst Case - Memory Usage (KB)**
算法跟Average Case一樣

| Size | Insertion_Memory | Quick_Memory | Merge_Memory | Heap_Memory |
| --- | --- | --- | --- | --- |
| 500 | 0 | 0.25 | 1.95312 | 0 |
| 1000 | 0 | 0.265625 | 3.90625 | 0 |
| 2000 | 0 | 0.28125 | 7.8125 | 0 |
| 3000 | 0 | 0.296875 | 11.7188 | 0 |
| 4000 | 0 | 0.28125 | 15.625 | 0 |
| 5000 | 0 | 0.34375 | 19.5312 | 0 |

![image](https://github.com/allurelove17/DataStructure-Homework/blob/main/homework1/worst_space_complexity_chart.png)

### 結論
根據line chart及table的數據可大致看出實際測試後的複雜度和理論上的複雜度是相符合的
使用Average Casse在所有測試規模下Quick Sort的表現都是最好的
使用Worst Case的情況下則是Merge Sort在所有測試規模下有最好的表現(!)

## 申論及開發報告
## Composite Sort
```c++
    double sort(int size, SIZE_T& memory_used) {
        auto start = std::chrono::high_resolution_clock::now();

        if (size <= 75) {
            for (int i = 0; i < size; ++i) {
                insertion_sorter->array[i] = array[i];
            }
            insertion_sorter->insertion_sort(size);
            for (int i = 0; i < size; ++i) {
                array[i] = insertion_sorter->array[i];
            }
            memory_used = size * sizeof(T) + sizeof(T);
        }
        else {
            for (int i = 0; i < size; ++i) {
                quick_sorter->array[i] = array[i];
            }
            SIZE_T quick_memory = 0;
            quick_sorter->quicktime(0, size - 1, quick_memory);
            for (int i = 0; i < size; ++i) {
                array[i] = quick_sorter->array[i];
            }
            memory_used = quick_memory;
        }

        auto end = std::chrono::high_resolution_clock::now();
        return std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    }
```
**1.Insertion Sort**

雖然Insertion Sort的理論時間複雜度並不好在但在Size很小的情況下Insertion Sort的表現是相當出色的
(Insertion Sort 相比於其他3個排序演算法並不需要額外調用資源,所以在Size很小的情況下實際執行時間是優於其他排序演算法的)

以下是每個Size各執行10000次的平均執行時間,可以發現當Size小於75時Insertion Sort的表現是穩定優於其他3個排序演算法的,所以當Size小於
75時Composite Sort會選擇使用Insertion Sort去對陣列進行排序

**Time Measurements (ms)**

| Size | Insertion_Time | Quick_Time | Merge_Time | Heap_Time |
| --- | --- | --- | --- | --- |
| 10 | 0.0000196 | 0.0000657 | 0.0000854 | 0.0000559 |
| 15 | 0.0000300 | 0.0000974 | 0.0001402 | 0.0001678 |
| 20 | 0.0000424 | 0.0001700 | 0.0006453 | 0.0007461 |
| 25 | 0.0000951 | 0.0006077 | 0.0011398 | 0.0011966 |
| 30 | 0.0001658 | 0.0011900 | 0.0011965 | 0.0012850 |
| 35 | 0.0002292 | 0.0012084 | 0.0013998 | 0.0015632 |
| 40 | 0.0011411 | 0.0012903 | 0.0018394 | 0.0022974 |
| 45 | 0.0012151 | 0.0013998 | 0.0022949 | 0.0024251 |
| 50 | 0.0013045 | 0.0019896 | 0.0024016 | 0.0025588 |
| 55 | 0.0016245 | 0.0023730 | 0.0024794 | 0.0034421 |
| 60 | 0.0021135 | 0.0025410 | 0.0028085 | 0.0036195 |
| 65 | 0.0024832 | 0.0026115 | 0.0035697 | 0.0038426 |
| 70 | 0.0026799 | 0.0028790 | 0.0037679 | 0.0046852 |
| 75 | 0.0032363 | 0.0033700 | 0.0040508 | 0.0047884 |
| 80 | 0.0035821 | 0.0035686 | 0.0046189 | 0.0051791 |
| 85 | 0.0039486 | 0.0036833 | 0.0047023 | 0.0055292 |
| 90 | 0.0046079 | 0.0038635 | 0.0048544 | 0.0060063 |
| 95 | 0.0049852 | 0.0044090 | 0.0054459 | 0.0066776 |
| 100 | 0.0055446 | 0.0047204 | 0.0058773 | 0.0071470 |

**2.Quick Sort**

根據以上的數據統計可以發現當Size不是極小值時,Quick sort無論測資大小為何都有著最好的表現所以當Size>75時Composite Sort 一律選擇使用Quick Sort進行排序,且median-of-three method可以避免worst case的情況發生這也降低了Composite Sort爆掉的風險

## Worst Case of Merge Sort
依照我實作的Merge Sort來看(以下為Merge Sort的部分程式碼),理論上只要能讓以下程式碼判斷式的執行次數盡量得多就可以得到Worst Case
```c++
while (i <= mid && j <= right) {
    if (temp_array[i] <= temp_array[j]) {
        array[k++] = temp_array[i++];
    }
    else {
        array[k++] = temp_array[j++];
    }
}
```
而Merge Sort執行的過程為將要排序的陣列分成好幾個區間,每個區間再剖半進行比較並將較小的數值放進array裡

所以根據Merge Sort的程式碼來看,只要正在排序的區間前1/2和後1/2能夠在上面的while loop裡讓i跟j的總和越大或k的數值越大,則判斷次數越多執行的效率就越差.而要達成這樣的目的必須左半邊和右半邊加入array的次數要盡量的平均

**Implementation**

根據以上的推論可以得知要想讓區間內左右兩邊的數值加入array的次數能夠平均,則陣列內的數值必須盡量是**交錯**的,以下的程式碼使用recursive的方式產生Merge Sort的Worst Case.

generate_merge_worst_case_recursive:

給定要產生的worst case大小,該程式碼會盡量地將worst case的大小平均的遞迴下去,然後再將結果合併起來

```c++
std::vector<int> generate_merge_worst_case_recursive(int n) {
    if (n <= 0) {
        return {}; 
    }
    if (n == 1) {
        return { 1 }; 
    }

    int k = std::ceil(static_cast<double>(n) / 2.0);
    int m = n - k;

    std::vector<int> left_part = generate_merge_worst_case_recursive(k);
    std::vector<int> right_part = generate_merge_worst_case_recursive(m);

    std::vector<int> result(n);
    int current_index = 0;

    for (int val : left_part) {
        result[current_index++] = 2 * val; // 確保數值是交錯的
    }

    for (int val : right_part) {
        result[current_index++] = 2 * val - 1; // 確保數值是交錯的
    }

    return result;
}
```

### Unexpected Results in Merge Sort Worst Case Analysis
## Why Merge Sort Worst Case better than Average Case?

以下為嘗試解決的辦法(全部失敗)
- 將Worst Case的產生方式重寫(換了4, 5種方法)
- 換電腦測試
- 改成用Linux執行
- 開啟O2優化

網路上找到的可能原因
- CPU會做Branch prediction

## 附錄
在Linux環境中用size_t

在Windows環境中用SIZE_T

#include <Windows.h>在include時必須在#include <psapi.h>前面不然會沒法執行
## 參考資料 // Option
