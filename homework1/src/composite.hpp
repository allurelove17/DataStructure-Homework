#ifndef COMPOSITE_SORT_HPP
#define COMPOSITE_SORT_HPP

template<class T>
class composite_sort {
public:
    composite_sort(int SIZE) {
        array = new T[SIZE];
    }

    ~composite_sort() {
        delete[] array;
    }

    void print(int size) {
        for (int i = 0; i < size; ++i) {
            std::cout << *(this->array + i) << ' ';
        }
        std::cout << '\n';
    }

    void getMemoryUsage(SIZE_T& workingSetSize) {
        PROCESS_MEMORY_COUNTERS_EX memInfo;
        GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&memInfo, sizeof(memInfo));
        workingSetSize = memInfo.PrivateUsage;
    }

    double sort(int size, SIZE_T& memory_used) {
        SIZE_T beforeMemory = 0, afterMemory = 0;
        getMemoryUsage(beforeMemory);

        auto start = std::chrono::high_resolution_clock::now();

        if (size <= 75) {
            insertion_sort(0, size - 1);
        }
        else {
            int depth = 0, max_depth = 0;
            quick_sort(0, size - 1, depth, max_depth);
            memory_used = max_depth * (3 * sizeof(int) + sizeof(T));
        }

        auto end = std::chrono::high_resolution_clock::now();

        getMemoryUsage(afterMemory);
        if (afterMemory > beforeMemory) {
            memory_used = afterMemory - beforeMemory;
        }

        return std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    }

    void insertion_sort(int left, int right) {
        for (int i = left + 1; i <= right; ++i) {
            T value = array[i];
            int j = i - 1;

            while (j >= left && value < array[j]) {
                array[j + 1] = array[j];
                j--;
            }

            array[j + 1] = value;
        }
    }

    int findMedianOfThree(int left, int mid, int right) {
        T a = array[left];
        T b = array[mid];
        T c = array[right];

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
                T temp = array[medianIndex];
                array[medianIndex] = array[right];
                array[right] = temp;
            }
        }

        T p = array[right];
        int i = left - 1, j = left;

        while (j < right) {
            if (array[j] < p) {
                ++i;
                T c = array[i];
                array[i] = array[j];
                array[j] = c;
            }
            ++j;
        }

        ++i;
        T c = array[i];
        array[i] = array[right];
        array[right] = c;

        return i;
    }

    void quick_sort(int left, int right, int depth, int& max_depth) {
        if (depth > max_depth) {
            max_depth = depth;
        }
        
        if (right - left <= 16) {
            insertion_sort(left, right);
            return;
        }

        if (left < right) {
            int p = pivot(left, right);
            quick_sort(left, p - 1, depth + 1, max_depth);
            quick_sort(p + 1, right, depth + 1, max_depth);
        }
    }

    T* array;
};

#endif