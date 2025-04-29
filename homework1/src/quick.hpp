#ifndef QUICK_HPP
#define QUICK_HPP

template<class T>
class quick {
public:
    quick(int SIZE) {
        array = new T[SIZE];
    }

    void print(int size) {
        for (int i = 0; i < size; ++i) {
            std::cout << *(this->array + i) << ' ';
        }std::cout << '\n';
    }

    int pivot(int left, int right) {
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

    void quick_sort(int left, int right, SIZE_T& memory_used, int depth, int& max_depth) {
        int q;
        while (left < right) {
            q = pivot(left, right);
            if (q - left < right - q) {
                quick_sort(left, q - 1, memory_used, depth + 1, max_depth);
                left = q + 1;
            }
            else {
                quick_sort(q + 1, right, memory_used, depth + 1, max_depth);
                right = q - 1;
            }
        }
        if (depth > max_depth)max_depth = depth;
        //size_t stack_frame_size = 3 * sizeof(int) + sizeof(T);
    }

    double quicktime(int left, int right, SIZE_T& memory_used) {
        int depth = 0, max_depth = 0;
        auto start = std::chrono::high_resolution_clock::now();
        quick_sort(left, right, memory_used, depth, max_depth);
        memory_used = max_depth * 3 * sizeof(int) + sizeof(T);
        auto end = std::chrono::high_resolution_clock::now();
        return std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    }

    T* array;
private:
};
#endif
