#ifndef COMPOSITE_HPP
#define COMPOSITE_HPP

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
        return std::chrono::duration<double, std::milli>(end - start).count();
    }

    T* array;
private:
    quick<T>* quick_sorter;
    heap<T>* heap_sorter;
    Merge<T>* merge_sorter;
    insertion<T>* insertion_sorter;
};

#endif
