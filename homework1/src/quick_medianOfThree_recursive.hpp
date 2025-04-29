#ifndef QUICK_HPP
#define QUICK_HPP

template<class T>
class quick {
public:
	quick(int SIZE) {
		array = new T[SIZE];
	}

	~quick() {
		delete []array;
	}

	void getMemoryUsage(SIZE_T& workingSetSize) {
		PROCESS_MEMORY_COUNTERS_EX memInfo;
		GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&memInfo, sizeof(memInfo));
		workingSetSize = memInfo.PrivateUsage;
	}

	void print(int size) {
		for (int i = 0; i < size; ++i) {
			std::cout << *(this->array + i) << ' ';
		}std::cout << '\n';
	}

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
			//getMemoryUsageOfCallStack(quick_memory);
			quick_sort(left, p - 1, quick_memory, depth + 1, max_depth);
			quick_sort(p + 1, right, quick_memory, depth + 1, max_depth);
		}
	}

	double quicktime(int left, int right, SIZE_T& quick_memory) {
		int depth = 0, max_depth = 0;
		SIZE_T beforeMemory = 0, afterMemory = 0;

		getMemoryUsage(beforeMemory);
		auto start = std::chrono::high_resolution_clock::now();
		quick_sort(left, right, quick_memory, depth, max_depth);
		auto end = std::chrono::high_resolution_clock::now();
		getMemoryUsage(afterMemory);

		if (afterMemory > beforeMemory) {
			quick_memory = afterMemory - beforeMemory;
		}
		else {
			// std::cout << "Calculate Quick Memory Usage.\n";
			quick_memory = max_depth * (3 * sizeof(int) + sizeof(T));
		}

		return std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
	}

	T* array;
private:
};
#endif
