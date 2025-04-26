#include <fstream>
#include <vector>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <chrono>
#include <algorithm>
#include <random>
#include <map>
#include <Windows.h> // GetCurrentProcess !!Comes before Psapi.h!!
#include <Psapi.h>
#include <cmath>
#include "insertion.hpp"
#include "quick_medianOfThree_recursive.hpp"
#include "Merge.hpp"
#include "heap.hpp"
#include "permutation.hpp"

template<class T>
class improved_quick {
public:
    improved_quick(int SIZE) {
        array = new T[SIZE];
    }

    ~improved_quick() {
        delete[] array;
    }

    void print(int size) {
        for (int i = 0; i < size; ++i) {
            std::cout << *(this->array + i) << ' ';
        }std::cout << '\n';
    }

    int pivot(int left, int right) {
        int mid = left + (right - left) / 2;

        if (array[left] > array[mid])
            std::swap(array[left], array[mid]);
        if (array[left] > array[right])
            std::swap(array[left], array[right]);
        if (array[mid] > array[right])
            std::swap(array[mid], array[right]);

        std::swap(array[mid], array[right - 1]);

        int pivotValue = array[right - 1];
        int i = left;

        for (int j = left; j < right - 1; j++) {
            if (array[j] <= pivotValue) {
                std::swap(array[i], array[j]);
                i++;
            }
        }

        std::swap(array[i], array[right - 1]);
        return i;
    }

    void quick_sort(int left, int right) {
        if (right - left <= 20) {
            for (int i = left + 1; i <= right; i++) {
                T key = array[i];
                int j = i - 1;
                while (j >= left && array[j] > key) {
                    array[j + 1] = array[j];
                    j--;
                }
                array[j + 1] = key;
            }
            return;
        }

        if (left < right) {
            int p = pivot(left, right);
            quick_sort(left, p - 1);
            quick_sort(p + 1, right);
        }
    }

    double quicktime(int left, int right) {
        auto start = std::chrono::high_resolution_clock::now();
        quick_sort(left, right);
        auto end = std::chrono::high_resolution_clock::now();
        return std::chrono::duration<double, std::milli>(end - start).count();
    }

    T* array;
};

template<class T>
class composite_sort {
public:
    composite_sort(int SIZE) {
        array = new T[SIZE];
        quick_sorter = new improved_quick<T>(SIZE);
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

    T* array;
private:
    improved_quick<T>* quick_sorter;
    heap<T>* heap_sorter;
    Merge<T>* merge_sorter;
    insertion<T>* insertion_sorter;
};

template<typename T>
double instrumented_insertion_sort(insertion<T>& sorter, int size, SIZE_T& memory_used) {
    memory_used = size * sizeof(T);
    memory_used += sizeof(T);

    auto start = std::chrono::high_resolution_clock::now();
    sorter.insertion_sort(size);
    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration<double, std::milli>(end - start).count();
}

template<typename T>
double instrumented_quick_sort(improved_quick<T>& sorter, int left, int right, SIZE_T& memory_used) {
    memory_used = (right - left + 1) * sizeof(T);

    int height = static_cast<int>(log2(right - left + 1));
    memory_used += height * (3 * sizeof(int)); // Stack frames ~ 3 integers per frame

    return sorter.quicktime(left, right);
}

template<typename T>
double instrumented_merge_sort(Merge<T>& sorter, int left, int right, SIZE_T& memory_used) {
    memory_used = (right - left + 1) * sizeof(T);
    memory_used += (right - left + 1) * sizeof(T);

    auto start = std::chrono::high_resolution_clock::now();
    sorter.merge_sort(left, right);
    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration<double, std::milli>(end - start).count();
}

template<typename T>
double instrumented_heap_sort(heap<T>& sorter, int size, SIZE_T& memory_used) {
    memory_used = size * sizeof(T);
    memory_used += 2 * sizeof(int);

    auto start = std::chrono::high_resolution_clock::now();
    sorter.heap_sort(size);
    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration<double, std::milli>(end - start).count();
}

template<typename T>
double instrumented_composite_sort(composite_sort<T>& sorter, int size, SIZE_T& memory_used) {
    return sorter.sort(size, memory_used);
}

std::vector<int> generate_worst_case_quick(int n) {
    std::vector<int> result(n);
    for (int i = 0; i < n; i++) {
        result[i] = n - i; // Reverse sorted array
    }
    return result;
}

int main() {
    std::vector<int> sizes = { 500, 1000, 2000, 3000, 4000, 5000, 10000, 20000 };

    std::random_device rd;
    std::mt19937 g(rd());

    std::cout << "Choose test mode:" << std::endl;
    std::cout << "1. Worst case analysis" << std::endl;
    std::cout << "2. Average case analysis" << std::endl;
    std::cout << "3. Both" << std::endl;
    std::cout << "Enter choice (1-3): ";

    int choice;
    std::cin >> choice;

    bool run_worst_case = (choice == 1 || choice == 3);
    bool run_average_case = (choice == 2 || choice == 3);

    if (run_worst_case) {
        std::cout << "Beginning worst case analysis..." << std::endl;

        std::map<int, double> worst_insertion_times;
        std::map<int, double> worst_quick_times;
        std::map<int, double> worst_merge_times;
        std::map<int, double> worst_heap_times;
        std::map<int, double> worst_composite_times;

        std::map<int, SIZE_T> worst_insertion_memory;
        std::map<int, SIZE_T> worst_quick_memory;
        std::map<int, SIZE_T> worst_merge_memory;
        std::map<int, SIZE_T> worst_heap_memory;
        std::map<int, SIZE_T> worst_composite_memory;

        for (auto size : sizes) {
            worst_insertion_times[size] = 0.0;
            worst_quick_times[size] = 0.0;
            worst_merge_times[size] = 0.0;
            worst_heap_times[size] = 0.0;
            worst_composite_times[size] = 0.0;

            worst_insertion_memory[size] = 0;
            worst_quick_memory[size] = 0;
            worst_merge_memory[size] = 0;
            worst_heap_memory[size] = 0;
            worst_composite_memory[size] = 0;
        }

        for (int runtime = 0; runtime < 10; ++runtime) {
            std::cout << "Run " << runtime + 1 << " of 10" << std::endl;

            for (auto size : sizes) {
                insertion<int> arr_insertion(size);
                improved_quick<int> arr_quick(size);
                Merge<int> arr_merge(size);
                heap<int> arr_heap(size);
                composite_sort<int> arr_composite(size);

                for (int i = 0; i < size; ++i) {
                    arr_insertion.array[i] = size - i;
                }

                std::vector<int> worst_quick = generate_worst_case_quick(size);
                for (int i = 0; i < size; ++i) {
                    arr_quick.array[i] = worst_quick[i];
                }

                try {
                    std::ifstream worstmergeifs("worstmerge.txt", std::ios::in);
                    std::stringstream ss_merge;
                    ss_merge << worstmergeifs.rdbuf();
                    worstmergeifs.close();

                    int x, ind = 0;
                    while (ss_merge >> x && ind < size) {
                        arr_merge.array[ind++] = x;
                    }

                    while (ind < size) {
                        arr_merge.array[ind] = size - ind;
                        ind++;
                    }
                }
                catch (...) {
                    for (int i = 0; i < size; ++i) {
                        arr_merge.array[i] = size - i;
                    }
                }

                switch (runtime % 3) {
                case 0: {
                    for (int i = 0; i < size; ++i) {
                        if (i < size / 2) {
                            arr_heap.array[i] = i;
                        }
                        else {
                            arr_heap.array[i] = size + size / 2 - i;
                        }
                    }
                    break;
                }
                case 1: {
                    for (int i = 0; i < size; ++i) {
                        arr_heap.array[i] = i + 1;
                    }
                    break;
                }
                case 2: {
                    for (int i = 0; i < size; ++i) {
                        arr_heap.array[i] = size - i;
                    }
                    break;
                }
                }

                for (int i = 0; i < size; ++i) {
                    arr_composite.array[i] = worst_quick[i];
                }

                SIZE_T insertion_memory = 0;
                SIZE_T quick_memory = 0;
                SIZE_T merge_memory = 0;
                SIZE_T heap_memory = 0;
                SIZE_T composite_memory = 0;

                double insertion_time = instrumented_insertion_sort(arr_insertion, size, insertion_memory);
                double quick_time = instrumented_quick_sort(arr_quick, 0, size - 1, quick_memory);
                double merge_time = instrumented_merge_sort(arr_merge, 0, size - 1, merge_memory);
                double heap_time = instrumented_heap_sort(arr_heap, size, heap_memory);
                double composite_time = instrumented_composite_sort(arr_composite, size, composite_memory);

                if (insertion_time > worst_insertion_times[size]) {
                    worst_insertion_times[size] = insertion_time;
                    worst_insertion_memory[size] = insertion_memory;
                }

                if (quick_time > worst_quick_times[size]) {
                    worst_quick_times[size] = quick_time;
                    worst_quick_memory[size] = quick_memory;
                }

                if (merge_time > worst_merge_times[size]) {
                    worst_merge_times[size] = merge_time;
                    worst_merge_memory[size] = merge_memory;
                }

                if (heap_time > worst_heap_times[size]) {
                    worst_heap_times[size] = heap_time;
                    worst_heap_memory[size] = heap_memory;
                }

                if (composite_time > worst_composite_times[size]) {
                    worst_composite_times[size] = composite_time;
                    worst_composite_memory[size] = composite_memory;
                }
            }
        }

        std::ofstream WorstCaseResults("sorting_worstcase.csv");
        WorstCaseResults << "Size,Insertion_Time(ms),Insertion_Memory(KB),Quick_Time(ms),Quick_Memory(KB),"
            << "Merge_Time(ms),Merge_Memory(KB),Heap_Time(ms),Heap_Memory(KB),Composite_Time(ms),Composite_Memory(KB)\n";

        for (auto size : sizes) {
            WorstCaseResults << size << ","
                << worst_insertion_times[size] << ","
                << (worst_insertion_memory[size] / 1024.0) << ","
                << worst_quick_times[size] << ","
                << (worst_quick_memory[size] / 1024.0) << ","
                << worst_merge_times[size] << ","
                << (worst_merge_memory[size] / 1024.0) << ","
                << worst_heap_times[size] << ","
                << (worst_heap_memory[size] / 1024.0) << ","
                << worst_composite_times[size] << ","
                << (worst_composite_memory[size] / 1024.0) << '\n';
        }

        WorstCaseResults.close();
        std::cout << "Worst case results saved to sorting_worstcase.csv" << std::endl;
    }

    if (run_average_case) {
        std::cout << "Beginning average case analysis..." << std::endl;

        std::map<int, double> total_insertion_times;
        std::map<int, double> total_quick_times;
        std::map<int, double> total_merge_times;
        std::map<int, double> total_heap_times;
        std::map<int, double> total_composite_times;

        std::map<int, SIZE_T> total_insertion_memory;
        std::map<int, SIZE_T> total_quick_memory;
        std::map<int, SIZE_T> total_merge_memory;
        std::map<int, SIZE_T> total_heap_memory;
        std::map<int, SIZE_T> total_composite_memory;

        for (auto size : sizes) {
            total_insertion_times[size] = 0.0;
            total_quick_times[size] = 0.0;
            total_merge_times[size] = 0.0;
            total_heap_times[size] = 0.0;
            total_composite_times[size] = 0.0;

            total_insertion_memory[size] = 0;
            total_quick_memory[size] = 0;
            total_merge_memory[size] = 0;
            total_heap_memory[size] = 0;
            total_composite_memory[size] = 0;
        }

        const int NUM_RUNS = 1000;

        for (int runtime = 0; runtime < NUM_RUNS; ++runtime) {
            if (runtime % 10 == 0) {
                std::cout << "Run " << runtime + 1 << " of " << NUM_RUNS << std::endl;
            }

            for (auto size : sizes) {
                insertion<int> arr_insertion(size);
                improved_quick<int> arr_quick(size);
                Merge<int> arr_merge(size);
                heap<int> arr_heap(size);
                composite_sort<int> arr_composite(size);

                std::vector<int> data(size);
                for (int i = 0; i < size; ++i) {
                    data[i] = i + 1;
                }
                std::shuffle(data.begin(), data.end(), g);

                for (int i = 0; i < size; ++i) {
                    arr_insertion.array[i] = data[i];
                    arr_quick.array[i] = data[i];
                    arr_merge.array[i] = data[i];
                    arr_heap.array[i] = data[i];
                    arr_composite.array[i] = data[i];
                }

                SIZE_T insertion_memory = 0;
                SIZE_T quick_memory = 0;
                SIZE_T merge_memory = 0;
                SIZE_T heap_memory = 0;
                SIZE_T composite_memory = 0;

                double insertion_time = instrumented_insertion_sort(arr_insertion, size, insertion_memory);
                double quick_time = instrumented_quick_sort(arr_quick, 0, size - 1, quick_memory);
                double merge_time = instrumented_merge_sort(arr_merge, 0, size - 1, merge_memory);
                double heap_time = instrumented_heap_sort(arr_heap, size, heap_memory);
                double composite_time = instrumented_composite_sort(arr_composite, size, composite_memory);

                total_insertion_times[size] += insertion_time;
                total_quick_times[size] += quick_time;
                total_merge_times[size] += merge_time;
                total_heap_times[size] += heap_time;
                total_composite_times[size] += composite_time;

                total_insertion_memory[size] += insertion_memory;
                total_quick_memory[size] += quick_memory;
                total_merge_memory[size] += merge_memory;
                total_heap_memory[size] += heap_memory;
                total_composite_memory[size] += composite_memory;
            }
        }

        std::ofstream AverageCaseResults("sorting_averagecase.csv");
        AverageCaseResults << "Size,Insertion_Time(ms),Insertion_Memory(KB),Quick_Time(ms),Quick_Memory(KB),"
            << "Merge_Time(ms),Merge_Memory(KB),Heap_Time(ms),Heap_Memory(KB),Composite_Time(ms),Composite_Memory(KB)\n";

        for (auto size : sizes) {
            double avg_insertion_time = total_insertion_times[size] / NUM_RUNS;
            double avg_quick_time = total_quick_times[size] / NUM_RUNS;
            double avg_merge_time = total_merge_times[size] / NUM_RUNS;
            double avg_heap_time = total_heap_times[size] / NUM_RUNS;
            double avg_composite_time = total_composite_times[size] / NUM_RUNS;

            double avg_insertion_memory = (total_insertion_memory[size] / NUM_RUNS) / 1024.0;
            double avg_quick_memory = (total_quick_memory[size] / NUM_RUNS) / 1024.0;
            double avg_merge_memory = (total_merge_memory[size] / NUM_RUNS) / 1024.0;
            double avg_heap_memory = (total_heap_memory[size] / NUM_RUNS) / 1024.0;
            double avg_composite_memory = (total_composite_memory[size] / NUM_RUNS) / 1024.0;

            AverageCaseResults << size << ","
                << avg_insertion_time << ","
                << avg_insertion_memory << ","
                << avg_quick_time << ","
                << avg_quick_memory << ","
                << avg_merge_time << ","
                << avg_merge_memory << ","
                << avg_heap_time << ","
                << avg_heap_memory << ","
                << avg_composite_time << ","
                << avg_composite_memory << '\n';
        }

        AverageCaseResults.close();
        std::cout << "Average case results saved to sorting_averagecase.csv" << std::endl;
    }

    std::cout << "All tests completed!" << std::endl;
    return 0;
}
