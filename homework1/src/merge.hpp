#ifndef MERGE_HPP
#define MERGE_HPP

template<class T>
class Merge {
public:
    Merge(int SIZE) {
        array = new T[SIZE];
        temp_array = new T[SIZE];
    }

    ~Merge() {
        delete[] array;
        delete[] temp_array;
    }

    void getMemoryUsage(SIZE_T& workingSetSize) {
        PROCESS_MEMORY_COUNTERS_EX memInfo;
        GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&memInfo, sizeof(memInfo));
        workingSetSize = memInfo.PrivateUsage;
    }

    double mergetime(int left, int right, SIZE_T& merge_memory) {
        for (int i = 0; i <= right - left; i++) {
            temp_array[i] = 0;
        }

        SIZE_T beforeMemory = 0;
        getMemoryUsage(beforeMemory);

        auto start = std::chrono::high_resolution_clock::now();
        merge_sort(left, right);
        auto end = std::chrono::high_resolution_clock::now();

        SIZE_T afterMemory = 0;
        getMemoryUsage(afterMemory);

        if (afterMemory <= beforeMemory) {
            // std::cout << "Calculate Merge Memory Usage.\n";
            merge_memory = (right - left + 1) * sizeof(T);
        }
        else {
            merge_memory = afterMemory - beforeMemory;
        }

        return std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    }

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

    void print(int size) {
        for (int i = 0; i < size; ++i) {
            std::cout << *(this->array + i) << ' ';
        }
        std::cout << '\n';
    }

    T* array;
private:
    T* temp_array;
};
#endif
