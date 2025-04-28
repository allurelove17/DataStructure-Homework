template<class T>
class composite_sort {
public:
    composite_sort(int SIZE) {
        array = new T[SIZE];
        quick_sorter = new quick<T>(SIZE);
        heap_sorter = new heap<T>(SIZE);
        merge_sorter = new Merge<T>(SIZE);
        insertion_sorter = new insertion<T>(SIZE);
    }

    ~composite_sort() {
        delete[] array;
        delete quick_sorter;
        delete heap_sorter;
        delete merge_sorter;
        delete insertion_sorter;
    }

    void print(int size) {
        for (int i = 0; i < size; ++i) {
            std::cout << *(this->array + i) << ' ';
        }
        std::cout << '\n';
    }

    double sort(int size, SIZE_T& memory_used) {
        auto start = std::chrono::high_resolution_clock::now();

        int max_depth = 2 * log2(size);

        if (size <= 50) {
            for (int i = 0; i < size; ++i) {
                insertion_sorter->array[i] = array[i];
            }
            insertion_sorter->insertion_sort(size);
            for (int i = 0; i < size; ++i) {
                array[i] = insertion_sorter->array[i];
            }
            memory_used = size * sizeof(T) + sizeof(T);
        }
        else if (is_nearly_sorted(size)) {
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
        else {
            for (int i = 0; i < size; ++i) {
                quick_sorter->array[i] = array[i];
            }

            SIZE_T quick_memory = 0;
            int depth = 0, max_path_depth = 0;

            introspective_sort(0, size - 1, depth, max_depth, max_path_depth, quick_memory);

            memory_used = quick_memory;
        }

        auto end = std::chrono::high_resolution_clock::now();
        return std::chrono::duration<double, std::milli>(end - start).count();
    }

    void introspective_sort(int left, int right, int depth, int max_depth, int& max_path_depth, SIZE_T& memory_used) {
        if (depth > max_path_depth) {
            max_path_depth = depth;
        }

        if (right - left <= 16) {
            for (int i = 0; i < right - left + 1; ++i) {
                insertion_sorter->array[i] = quick_sorter->array[i + left];
            }
            insertion_sorter->insertion_sort(right - left + 1);
            for (int i = 0; i < right - left + 1; ++i) {
                quick_sorter->array[i + left] = insertion_sorter->array[i];
            }
            return;
        }

        if (depth >= max_depth) {
            for (int i = left; i <= right; ++i) {
                heap_sorter->array[i - left] = quick_sorter->array[i];
            }

            heap_sorter->heap_sort(right - left + 1);

            for (int i = left; i <= right; ++i) {
                quick_sorter->array[i] = heap_sorter->array[i - left];
            }

            return;
        }

        int p = quick_sorter->pivot(left, right);

        introspective_sort(left, p - 1, depth + 1, max_depth, max_path_depth, memory_used);
        introspective_sort(p + 1, right, depth + 1, max_depth, max_path_depth, memory_used);
    }

    bool is_nearly_sorted(int size) {
        int max_inversions = size / 20;
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

    T* array;
private:
    quick<T>* quick_sorter;
    heap<T>* heap_sorter;
    Merge<T>* merge_sorter;
    insertion<T>* insertion_sorter;
};