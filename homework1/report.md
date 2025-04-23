# 41243241 彭毓升 41243233 林厚丞

作業一 sorting

## 解題說明

本次作業的目標是比較四種常見的排序演算法：Insertion Sort、Quick Sort、Merge Sort 和 Heap Sort，在不同輸入大小下的執行效能表現。每種排序法在平均情況與最壞情況下的效能，並繪製圖表進行分析。


## 實驗方法  
### 測量方式  
- 使用C++實現，搭配`chrono`庫測量執行時間（毫秒）  
- 輸入規模n從500開始，逐步增加至5000（部分測試到20000）  


### 測試情境  
- **平均情況**：隨機排列的數據  
- **最壞情況**：針對各演算法特性設計的特殊數據
## 程式實作


**Insertion Sort**

```cpp
#include "insertion.hpp"

template<class T>
void insertion<T>::print(int size) {
	for (int i = 0; i < size; ++i) {
		std::cout << *(this->array + i) << ' ';
	}std::cout << '\n';
}

template<class T>
double insertion<T>::insertion_sort(int size) {
	auto start = std::chrono::steady_clock::now();
	for (int i = 1; i < size; ++i) {
		int value = *(this->array + i), j;
		for (j = i - 1; j >= 0 && value < *(this->array + j); --j) {
			if (value < *(this->array + j))
				*(this->array + j + 1) = *(this->array + j);
		}
		*(this->array + j + 1) = value;
	}
	auto end = std::chrono::steady_clock::now();
	return std::chrono::duration<double, std::milli>(end - start).count();
}

template class insertion<int>;
```

**Quick Sort**

```cpp
#include "quick.hpp"

template<class T>
void quick<T>::swap(T& a, T& b) {
	T c = a;
	a = b;
	b = c;
}

template<class T>
void quick<T>::print(int size) {
	for (int i = 0; i < size; ++i) {
		std::cout << *(this->array + i) << ' ';
	}std::cout << '\n';
}

template<class T>
int quick<T>::pivot(int left, int right) {
	int p = *(this->array + right), i = left - 1, j = left;
	T c;
	while (j < right) {
		if (*(this->array + j) < p) {
			++i;
			//swap((this->array + i), (this->array + j));
			c = *(this->array + i);
			*(this->array + i) = *(this->array + j);
			*(this->array + j) = c;
			
		}
		++j;
	}
	++i;
	//swap((this->array + i), (this->array + right));
	c = *(this->array + i);
	*(this->array + i) = *(this->array + right);
	*(this->array + right) = c;
	return i;
}

template<class T>
double quick<T>::quick_sort(int left, int right) {
	auto start = std::chrono::steady_clock::now();
	if (left < right) {
		int p = pivot(left, right);
		quick_sort(left, p - 1);
		quick_sort(p + 1, right);
	}
	auto end = std::chrono::steady_clock::now();
	return std::chrono::duration<double, std::milli>(end - start).count();
}

template class quick<int>;
```

**Merge Sort**

```cpp
#include "Merge.hpp"

template<class T>
void quick<T>::swap(T& a, T& b) {
	T c = a;
	a = b;
	b = c;
}

template<class T>
void Merge<T>::print(int size) {
	for (int i = 0; i < size; ++i) {
		std::cout << *(this->array + i) << ' ';
	}std::cout << '\n';
}

template<class T>
void Merge<T>::merge(int left, int mid, int right) {
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

template<class T>
void Merge<T>::merge_sort(int left, int right) {
	if (left < right) {
		int mid = (left + right) / 2;
		merge_sort(left, mid);
		merge_sort(mid + 1, right);
		merge(left, mid, right);
	}
}

template class Merge<int>;
```

**Heap Sort**

```cpp
#include "heap.hpp"

template<class T>
void quick<T>::swap(T& a, T& b) {
	T c = a;
	a = b;
	b = c;
}

template<class T>
void heap<T>::print(int size) {
	for (int i = 0; i < size; ++i) {
		std::cout << *(this->array + i) << ' ';
	}std::cout << '\n';
}

template<class T>
void heap<T>::maxheap(int root, int length) {
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

template<class T>
void heap<T>::build(int size) {
	for (int i = size / 2; i >= 1; --i) {
		maxheap(i, size - 1);
	}
}

template<class T>
void heap<T>::heap_sort(int size) {
	T c;
	*(this->array) = 0;

	build(size);

	int len = size - 1;
	for (int i = size - 1; i >= 2; --i) {
		c = *(this->array + 1);
		*(this->array + 1) = *(this->array + i);
		*(this->array + i) = c;
		--len;
		maxheap(1, len);
	}
	for (int i = 0; i < size - 1; i++) {
		*(this->array + i) = *(this->array + i + 1);
	}
}

template class heap<int>;
```

## 效能分析

排序演算法的效能可以從以下兩個角度來分析：時間複雜度與空間複雜度。

**時間複雜度比較**

| 演算法 | 最佳情況| 平均情況 | 最壞情況 |
|----------|--------------|----------|----------|
| Insertion Sort   | O(n)     | O(n²)        | O(n²)       |
| Quick Sort   | O(n log n)      | O(n log n)        | O(n²)         |
| Merge Sort   | O(n log n)     | O(n log n)        | O(n log n)        |
| Heap Sort  | O(n log n)     | O(n log n)       | O(n log n)     |



### 實際測試數據

**平均情況（單位：毫秒）**

| 數據規模 | Insertion | Quick | Merge | Merge| 
|----------|--------------|----------|----------|----------|
|500|	0.16936|	0.041775	|1.2454|	0.053985|
|1000|	0.632015	|0.07899|	2.75133	|0.113925|
|2000|	2.26618	|0.181835	|5.28603	|0.25124|
|3000|	5.37174|	0.299585	|7.30325	|0.397875|
|4000|	9.61726|	0.4156	|10.7534|	0.58034|
|5000|	12.3864	|0.379175|	11.9326	|0.729085|
|10000|	54.7868	|0.908745|	24.5244|	1.39043|
|20000|	194.988	|2.09169|	46.2534|	3.61741|

**最壞情況（單位：毫秒）**
| 數據規模 | Insertion | Quick | Merge | Merge| 
|----------|--------------|----------|----------|----------|
|500|	0.254	|0.2465|	1.3432	|0.058|
|1000|	0.7912	|0.7751	|2.0162|	0.0762|
|2000|	7.8519	|6.8435|	13.7853|	0.4099|
|3000|	14.795	|9.5306|	10.3317|	0.323|
|4000|	16.8835	|14.9354	|10.847	|0.4794|
|5000|	24.0473	|25.0886	|11.2591	|0.4707|

### 結論

1. 小規模數據（n < 1000）

 -Insertion Sort因常數因子小，實際表現優於理論預測
 -Quick Sort展現最佳平均性能

2. 中大型數據（1000 ≤ n ≤ 5000）

  -Quick Sort保持領先（平均0.38ms@5000）
  -Heap Sort表現穩定，與Merge Sort交叉領先

3. 大規模數據（n > 10000）

  -Merge Sort展現O(n log n)優勢
  -Quick Sort最壞情況需警惕（25.09ms@5000）

4. 特殊場景建議

  -內存受限：Heap Sort（空間O(1)）
  -穩定性要求：Merge Sort
  -預測性需求：避免Quick Sort最壞情況


### 圖表

![平均情況比較](https://github.com/allurelove17/Data-Structure/blob/main/Sorting/sorting_average_comparison.png)
![最壞情況比較](https://github.com/allurelove17/Data-Structure/blob/main/Sorting/sorting_worst_comparison.png)
![最差情況與平均情況比較](https://github.com/allurelove17/Data-Structure/blob/main/Sorting/sorting_comparison_combined.png) 