#include <fstream>
#include <vector>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <chrono>
#include <algorithm>
#include <random>
#include <map>
#include <Windows.h> // GetCurrentProcess
#include <psapi.h>
#include <functional>
#include "insertion.hpp"
//#include "quick.hpp"
#include "quick_medianOfThree_recursive.hpp"
#include "Merge.hpp"
#include "heap.hpp"
#include "permutation.hpp"

// Composite sort class that intelligently selects the best algorithm
/*template<class T>
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

    double sort(int size) {
        // Copy data to all sorters
        for (int i = 0; i < size; ++i) {
            quick_sorter->array[i] = array[i];
            heap_sorter->array[i] = array[i];
            merge_sorter->array[i] = array[i];
            insertion_sorter->array[i] = array[i];
        }

        auto start = std::chrono::steady_clock::now();

        // Choose algorithm based on input size and array characteristics
        if (size <= 20) {
            // For very small arrays, insertion sort is efficient
            insertion_sorter->insertion_sort(size);
            // Copy result back
            for (int i = 0; i < size; ++i) {
                array[i] = insertion_sorter->array[i];
            }
        }
        else if (is_nearly_sorted(size)) {
            // For nearly sorted data, heap sort performs well
            heap_sorter->heap_sort(size);
            // Copy result back
            for (int i = 0; i < size; ++i) {
                array[i] = heap_sorter->array[i];
            }
        }
        else if (size <= 1000) {
            // For moderate size arrays, quick sort is very efficient
            quick_sorter->quick_sort(0, size - 1);
            // Copy result back
            for (int i = 0; i < size; ++i) {
                array[i] = quick_sorter->array[i];
            }
        }
        else {
            // For large arrays, select based on your performance data
            // Based on your data, quick sort is generally fastest
            quick_sorter->quick_sort(0, size - 1);
            // Copy result back
            for (int i = 0; i < size; ++i) {
                array[i] = quick_sorter->array[i];
            }
        }

        auto end = std::chrono::steady_clock::now();
        return std::chrono::duration<double, std::milli>(end - start).count();
    }

    // Helper function to detect if array is nearly sorted
    bool is_nearly_sorted(int size) {
        int max_inversions = size / 10; // Allow 10% of elements to be out of order
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
};*/

/*void getMemoryUsage(SIZE_T& workingSetSize) {
    PROCESS_MEMORY_COUNTERS memInfo;
    GetProcessMemoryInfo(GetCurrentProcess(), &memInfo, sizeof(memInfo));
    workingSetSize = memInfo.WorkingSetSize;
}*/

void getMemoryUsage(SIZE_T& workingSetSize) {
    PROCESS_MEMORY_COUNTERS_EX memInfo;
    GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&memInfo, sizeof(memInfo));
    workingSetSize = memInfo.PrivateUsage;
}

void getMemoryUsageOfCallStack(SIZE_T& workingSetSize) {
    PROCESS_MEMORY_COUNTERS memInfo;
    GetProcessMemoryInfo(GetCurrentProcess(), &memInfo, sizeof(memInfo));
    workingSetSize = memInfo.PeakWorkingSetSize;
}

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

std::vector<int> generate_merge_worst_case_recursive(int n) {
    if (n <= 0) {
        return {}; // ªÅ§Ç¦C
    }
    if (n == 1) {
        return { 1 }; // n=1 ªº°òÂ¦±¡ªp
    }

    // ±N n ¤À¦¨¨â¥b¡Ak ¬O«e¥b³¡¤Àªº¤j¤p (¦V¤W¨ú¾ã)
    int k = std::ceil(static_cast<double>(n) / 2.0);
    int m = n - k; // «á¥b³¡¤Àªº¤j¤p

    // »¼°j²£¥Í¥ª¥k¨â³¡¤Àªº¼Ò¦¡
    std::vector<int> left_part = generate_merge_worst_case_recursive(k);
    std::vector<int> right_part = generate_merge_worst_case_recursive(m);

    std::vector<int> result(n); // ³Ì²×µ²ªG§Ç¦C
    int current_index = 0;

    // ³B²z¥ª¥b³¡¤À¡G±N»¼°jµ²ªG¤¤ªº¨C­Ó­È x Âà´«¬° 2x
    for (int val : left_part) {
        result[current_index++] = 2 * val;
    }

    // ³B²z¥k¥b³¡¤À¡G±N»¼°jµ²ªG¤¤ªº¨C­Ó­È x Âà´«¬° 2x - 1
    for (int val : right_part) {
        result[current_index++] = 2 * val - 1;
    }

    return result; // ªð¦^ºc³y¦nªº§Ç¦C
}


template<class T>
std::vector<T> worstCaseOfmerge(int n) {
    if (n <= 0) return {};
    // »¼°j¨ç¼Æª½±µ¥Í¦¨©Ò»Ýªº 1..n ±Æ¦C
    return generate_merge_worst_case_recursive(n);
}

int main() {
    std::vector<int> sizes = { 500, 1000, 2000, 3000, 4000, 5000 };

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

        std::map<int, std::vector<int>> worst_quick_permutations;
        std::map<int, std::vector<int>> worst_heap_permutations;

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

            worst_quick_permutations[size] = std::vector<int>(size);
            worst_heap_permutations[size] = std::vector<int>(size);
        }

        for (auto size : sizes) { // separate
            SIZE_T beforeMemory = 0, afterMemory = 0;


            insertion<int> arr_insertion(size);

            for (int i = 0; i < size; ++i) {
                arr_insertion.array[i] = size - i;
            }
            getMemoryUsage(beforeMemory); //Insertion

            // double insertion_time = arr_insertion.insertion_sort(size);
            worst_insertion_times[size] = arr_insertion.insertion_sort(size);
            getMemoryUsage(afterMemory);
            SIZE_T insertion_memory = afterMemory - beforeMemory;
            worst_insertion_memory[size] = insertion_memory;



            /*quick<int> arr_quick(size);
            SIZE_T quick_memory = 0;// Quick Worstcase
            //getMemoryUsage(beforeMemory);
            for (int i = 0; i < size; ++i) {
                arr_quick.array[i] = i;
            }
            worst_quick_times[size] = arr_quick.quicktime(0, size - 1, quick_memory);
            //getMemoryUsageOfCallStack(quick_memory);
            //getMemoryUsage(afterMemory);
            //quick_memory = afterMemory - beforeMemory;
            worst_quick_memory[size] = quick_memory;*/
            // examine sorting correctness
            //if (size <= 1000) arr_quick.print(size);


            Merge<int> arr_merge(size);
            /*try {
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
            }*/
            const int WORST_RUNS = 100;
            double total_merge_time = 0.0, total_merge_mem = 0.0;
            for (int runtime = 0; runtime < WORST_RUNS; ++runtime) {
                std::vector<int> worstMerge = worstCaseOfmerge<int>(size);
                for (int i = 0; i < size; ++i) {
                    arr_merge.array[i] = worstMerge[i];
                }
                /*if (size <= 1000) {
                    for (int i = 0; i < size; ++i) {
                        std::cout << arr_merge.array[i] << ' ';
                    }
                }*/
                SIZE_T merge_memory = 0;
                getMemoryUsage(beforeMemory); // Merge
                // double merge_time = arr_merge.mergetime(0, size - 1);
                // worst_merge_times[size] = arr_merge.mergetime(0, size - 1, merge_memory);
                total_merge_time += arr_merge.mergetime(0, size - 1, merge_memory);
                getMemoryUsage(afterMemory);
                //merge_memory = afterMemory - beforeMemory;
                // worst_merge_memory[size] = merge_memory;
                total_merge_mem += merge_memory;
                // if (size <= 1000)arr_merge.print(size);
            }
            worst_merge_times[size] = total_merge_time / WORST_RUNS;
            worst_merge_memory[size] = total_merge_mem / WORST_RUNS;


            double worst_quick_time = 0.0, worst_heap_time = 0.0;
            SIZE_T final_quick_memory = 0, final_heap_memory = 0;
            for (int runtime = 0; runtime < 100; ++runtime) {
                std::vector<int> worst = permutation(size);
                // Quick

                quick<int> arr_quick(size);
                SIZE_T quick_memory = 0;// Quick

                for (int i = 0; i < size; ++i) { // Random data for Quicksort
                    arr_quick.array[i] = worst[i];
                }
                double quick_time = arr_quick.quicktime(0, size - 1, quick_memory);

                if (quick_time > worst_quick_time) {
                    worst_quick_time = quick_time;
                    final_quick_memory = quick_memory;
                    for (int i = 0; i < size; ++i) {
                        worst_quick_permutations[size][i] = worst[i];
                    }
                }
                //==============================================================


                heap<int> arr_heap(size);
                getMemoryUsage(beforeMemory); // Heap
                for (int i = 0; i < size; ++i) {
                    arr_heap.array[i] = worst[i];
                }
                double heap_time = arr_heap.heap_sort(size);
                getMemoryUsage(afterMemory);
                SIZE_T heap_memory = afterMemory - beforeMemory;

                if (heap_time > worst_heap_time) {
                    worst_heap_time = heap_time;
                    final_heap_memory = heap_memory;
                    for (int i = 0; i < size; ++i) {
                        worst_heap_permutations[size][i] = worst[i];
                    }
                }
            }
            worst_quick_times[size] = worst_quick_time;
            worst_quick_memory[size] = final_quick_memory;
            worst_heap_times[size] = worst_heap_time;
            worst_heap_memory[size] = final_heap_memory;
        }

        std::ofstream WorstCaseResults("sorting_worstcase.csv");
        WorstCaseResults << "Size,Insertion_Time(ms),Insertion_Memory(KB),Quick_Time(ms),Quick_Memory(KB),"
            << "Merge_Time(ms),Merge_Memory(KB),Heap_Time(ms),Heap_Memory(KB),Composite_Time(ms),Composite_Memory(KB)\n";
        for (auto size : sizes) {
            WorstCaseResults << size << ","
                << worst_insertion_times[size] / 1000.0 << ","
                << (worst_insertion_memory[size] / 1024.0) << ","
                << worst_quick_times[size] / 1000.0 << ","
                << (worst_quick_memory[size] / 1024.0) << ","
                << worst_merge_times[size] / 1000.0 << ","
                << (worst_merge_memory[size] / 1024.0) << ","
                << worst_heap_times[size] / 1000.0 << ","
                << (worst_heap_memory[size] / 1024.0) << ","
                << worst_composite_times[size] / 1000.0 << ","
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
                quick<int> arr_quick(size);
                Merge<int> arr_merge(size);
                heap<int> arr_heap(size);
                //composite_sort<int> arr_composite(size);

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
                    //arr_composite.array[i] = data[i];
                }

                SIZE_T beforeMemory = 0, afterMemory = 0;
                SIZE_T insertion_memory = 0;
                SIZE_T quick_memory = 0;
                SIZE_T merge_memory = 0;
                SIZE_T heap_memory = 0;
                SIZE_T composite_memory = 0;

                getMemoryUsage(beforeMemory);
                double insertion_time = arr_insertion.insertion_sort(size);
                getMemoryUsage(afterMemory);
                insertion_memory = afterMemory - beforeMemory;

                double quick_time = arr_quick.quicktime(0, size - 1, quick_memory);

                // getMemoryUsage(beforeMemory);
                double merge_time = arr_merge.mergetime(0, size - 1, merge_memory);
                //getMemoryUsage(afterMemory);
                // merge_memory = afterMemory - beforeMemory;

                getMemoryUsage(beforeMemory);
                double heap_time = heap_time = arr_heap.heap_sort(size);
                getMemoryUsage(afterMemory);
                heap_memory = afterMemory - beforeMemory;
                //double composite_time = instrumented_composite_sort(arr_composite, size, composite_memory);

                total_insertion_times[size] += insertion_time;
                total_quick_times[size] += quick_time;
                total_merge_times[size] += merge_time;
                total_heap_times[size] += heap_time;
                //total_composite_times[size] += composite_time;

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
            double avg_insertion_time = total_insertion_times[size] / NUM_RUNS / 1000.0;
            double avg_quick_time = total_quick_times[size] / NUM_RUNS / 1000.0;
            double avg_merge_time = total_merge_times[size] / NUM_RUNS / 1000.0;
            double avg_heap_time = total_heap_times[size] / NUM_RUNS / 1000.0;
            double avg_composite_time = total_composite_times[size] / NUM_RUNS / 1000.0;

            double avg_insertion_memory = (total_insertion_memory[size] / NUM_RUNS / 1024.0);
            double avg_quick_memory = (total_quick_memory[size] / NUM_RUNS / 1024.0);
            double avg_merge_memory = (total_merge_memory[size] / NUM_RUNS / 1024.0);
            double avg_heap_memory = (total_heap_memory[size] / NUM_RUNS / 1024.0);
            double avg_composite_memory = (total_composite_memory[size] / NUM_RUNS / 1024.0);

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