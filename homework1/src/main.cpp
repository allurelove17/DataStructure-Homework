#include <fstream>
#include <vector>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <chrono>
#include <algorithm>
#include <random>
#include <map>
#include "insertion.hpp"
#include "quick.hpp"
#include "Merge.hpp"
#include "heap.hpp"
#include "permutation.hpp"

// Function to generate theoretical worst-case array for heap sort
std::vector<int> worstCaseHeapArrayOfSize(int n) {
    if (n == 1) {
        return { 1 };
    }
    else {
        int topSize = std::floor(static_cast<float>(n) / 2);
        int bottomSize = std::ceil(static_cast<float>(n) / 2);

        std::vector<int> top = worstCaseHeapArrayOfSize(topSize);
        std::vector<int> bottom = worstCaseHeapArrayOfSize(bottomSize);

        std::vector<int> result;
        result.reserve(n);

        // Multiply each element in top by 2
        for (int i = 0; i < top.size(); i++) {
            result.push_back(top[i] * 2);
        }

        // Multiply each element in bottom by 2 and subtract 1
        for (int i = 0; i < bottom.size(); i++) {
            result.push_back(bottom[i] * 2 - 1);
        }

        return result;
    }
}

int main() {
    std::vector<int> sizes = { 500, 1000, 2000, 3000, 4000, 5000, 10000, 20000 };

    // Create random number generator
    std::random_device rd;
    std::mt19937 g(rd());

    // Ask user which test to run
    std::cout << "Choose test mode:" << std::endl;
    std::cout << "1. Worst case analysis" << std::endl;
    std::cout << "2. Average case analysis" << std::endl;
    std::cout << "3. Both" << std::endl;
    std::cout << "Enter choice (1-3): ";

    int choice;
    std::cin >> choice;

    bool run_worst_case = (choice == 1 || choice == 3);
    bool run_average_case = (choice == 2 || choice == 3);

    // ===== WORST CASE ANALYSIS =====
    if (run_worst_case) {
        std::cout << "Beginning worst case analysis..." << std::endl;

        // For each size, we'll track the worst times for all algorithms
        std::map<int, double> worst_insertion_times;
        std::map<int, double> worst_quick_times;
        std::map<int, double> worst_merge_times;
        std::map<int, double> worst_heap_times;
        std::map<int, std::vector<int>> worst_heap_permutations;

        // Initialize with zeros
        for (auto size : sizes) {
            worst_insertion_times[size] = 0.0;
            worst_quick_times[size] = 0.0;
            worst_merge_times[size] = 0.0;
            worst_heap_times[size] = 0.0;
            worst_heap_permutations[size] = std::vector<int>(size);
        }

        // Run multiple iterations to find the worst case
        for (int runtime = 0; runtime < 10; ++runtime) {
            std::cout << "Run " << runtime + 1 << " of 10" << std::endl;

            for (auto size : sizes) {
                // Create sorting algorithm objects
                insertion<int> arr_insertion(size);
                quick<int> arr_quick(size);
                Merge<int> arr_merge(size);
                heap<int> arr_heap(size);

                // INSERTION SORT WORST CASE - Reverse sorted array
                for (int i = 0; i < size; ++i) {
                    arr_insertion.array[i] = size - i;
                }

                // QUICK SORT WORST CASE - Already sorted array
                for (int i = 0; i < size; ++i) {
                    arr_quick.array[i] = i;
                }

                // MERGE SORT WORST CASE - Load from file
                try {
                    std::ifstream worstmergeifs("worstmerge.txt", std::ios::in);
                    std::stringstream ss_merge;
                    ss_merge << worstmergeifs.rdbuf();
                    worstmergeifs.close();

                    int x, ind = 0;
                    while (ss_merge >> x && ind < size) {
                        arr_merge.array[ind++] = x;
                    }

                    // If file doesn't have enough elements, fill remaining
                    while (ind < size) {
                        arr_merge.array[ind] = size - ind;
                        ind++;
                    }
                }
                catch (...) {
                    // If file reading fails, use reverse sorted
                    for (int i = 0; i < size; ++i) {
                        arr_merge.array[i] = size - i;
                    }
                }

                // HEAP SORT - Try different permutation strategies for each run
                switch (runtime % 5) {
                case 0: {
                    // Try theoretical worst case
                    std::vector<int> worst = worstCaseHeapArrayOfSize(size);
                    for (int i = 0; i < size && i < worst.size(); ++i) {
                        arr_heap.array[i] = worst[i];
                    }
                    break;
                }
                case 1: {
                    // Try loading from worstinser.txt (as in your original code)
                    try {
                        std::ifstream worstinserifs("worstinser.txt", std::ios::in);
                        std::stringstream ss_inser;
                        ss_inser << worstinserifs.rdbuf();
                        worstinserifs.close();

                        int x, ind = 0;
                        while (ss_inser >> x && ind < size) {
                            arr_heap.array[ind++] = x;
                        }

                        // Fill remaining if needed
                        while (ind < size) {
                            arr_heap.array[ind] = ind;
                            ind++;
                        }
                    }
                    catch (...) {
                        // Fallback if file doesn't exist
                        for (int i = 0; i < size; ++i) {
                            arr_heap.array[i] = i;
                        }
                    }
                    break;
                }
                case 2: {
                    // Try a sawtooth pattern
                    for (int i = 0; i < size; ++i) {
                        arr_heap.array[i] = i % 2 == 0 ? i : size - i;
                    }
                    break;
                }
                case 3: {
                    // Try a random permutation
                    std::vector<int> data(size);
                    for (int i = 0; i < size; ++i) {
                        data[i] = i + 1;
                    }
                    std::shuffle(data.begin(), data.end(), g);
                    for (int i = 0; i < size; ++i) {
                        arr_heap.array[i] = data[i];
                    }
                    break;
                }
                case 4: {
                    // Try the currently known worst permutation
                    if (worst_heap_times[size] > 0) {
                        for (int i = 0; i < size; ++i) {
                            arr_heap.array[i] = worst_heap_permutations[size][i];
                        }
                        // Add small variations to see if we can make it worse
                        if (size > 10) {
                            std::swap(arr_heap.array[0], arr_heap.array[size - 1]);
                            std::swap(arr_heap.array[size / 3], arr_heap.array[2 * size / 3]);
                        }
                    }
                    else {
                        // If no known worst case yet, try reverse sorted
                        for (int i = 0; i < size; ++i) {
                            arr_heap.array[i] = size - i;
                        }
                    }
                    break;
                }
                }

                // Measure sort times
                double insertion_time = arr_insertion.insertion_sort(size);
                double quick_time = arr_quick.quicktime(0, size - 1);
                double merge_time = arr_merge.mergetime(0, size - 1);
                double heap_time = arr_heap.heap_sort(size);

                // Update worst times if we found worse ones
                if (insertion_time > worst_insertion_times[size]) {
                    worst_insertion_times[size] = insertion_time;
                }

                if (quick_time > worst_quick_times[size]) {
                    worst_quick_times[size] = quick_time;
                }

                if (merge_time > worst_merge_times[size]) {
                    worst_merge_times[size] = merge_time;
                }

                if (heap_time > worst_heap_times[size]) {
                    worst_heap_times[size] = heap_time;
                    for (int i = 0; i < size; ++i) {
                        worst_heap_permutations[size][i] = arr_heap.array[i];
                    }

                    // Save this new worst permutation to a file
                    std::ofstream worst_file("worst_heap_" + std::to_string(size) + ".txt");
                    for (int i = 0; i < size; ++i) {
                        worst_file << arr_heap.array[i] << " ";
                    }
                    worst_file.close();

                    std::cout << "New worst case for size " << size << ": " << heap_time << " ms" << std::endl;
                }
            }
        }

        // Now write only the worst-case results to the CSV file
        std::ofstream WorstCaseResults("sorting_worstcase.csv");

        for (auto size : sizes) {
            WorstCaseResults << size << ","
                << worst_insertion_times[size] << ","
                << worst_quick_times[size] << ","
                << worst_merge_times[size] << ","
                << worst_heap_times[size] << '\n';
        }

        WorstCaseResults.close();
        std::cout << "Worst case results saved to sorting_worstcase.csv" << std::endl;
    }

    // ===== AVERAGE CASE ANALYSIS =====
    if (run_average_case) {
        std::cout << "Beginning average case analysis..." << std::endl;

        // For each size, track total times and number of runs
        std::map<int, double> total_insertion_times;
        std::map<int, double> total_quick_times;
        std::map<int, double> total_merge_times;
        std::map<int, double> total_heap_times;

        // Initialize with zeros
        for (auto size : sizes) {
            total_insertion_times[size] = 0.0;
            total_quick_times[size] = 0.0;
            total_merge_times[size] = 0.0;
            total_heap_times[size] = 0.0;
        }

        // Number of runs for averaging
        const int NUM_RUNS = 20;

        // Run multiple iterations to find the average case
        for (int runtime = 0; runtime < NUM_RUNS; ++runtime) {
            std::cout << "Run " << runtime + 1 << " of " << NUM_RUNS << std::endl;

            for (auto size : sizes) {
                // Create sorting algorithm objects
                insertion<int> arr_insertion(size);
                quick<int> arr_quick(size);
                Merge<int> arr_merge(size);
                heap<int> arr_heap(size);

                // Generate random data for average case
                std::vector<int> data(size);
                for (int i = 0; i < size; ++i) {
                    data[i] = i + 1;
                }
                std::shuffle(data.begin(), data.end(), g);

                // Copy same data to all algorithms for fair comparison
                for (int i = 0; i < size; ++i) {
                    arr_insertion.array[i] = data[i];
                    arr_quick.array[i] = data[i];
                    arr_merge.array[i] = data[i];
                    arr_heap.array[i] = data[i];
                }

                // Measure sort times
                double insertion_time = arr_insertion.insertion_sort(size);
                double quick_time = arr_quick.quicktime(0, size - 1);
                double merge_time = arr_merge.mergetime(0, size - 1);
                double heap_time = arr_heap.heap_sort(size);

                // Add to running totals
                total_insertion_times[size] += insertion_time;
                total_quick_times[size] += quick_time;
                total_merge_times[size] += merge_time;
                total_heap_times[size] += heap_time;

                std::cout << "Size " << size << " - Run " << runtime + 1 << " complete" << std::endl;
            }
        }

        // Now write the average-case results to the CSV file
        std::ofstream AverageCaseResults("sorting_averagecase.csv");

        for (auto size : sizes) {
            double avg_insertion = total_insertion_times[size] / NUM_RUNS;
            double avg_quick = total_quick_times[size] / NUM_RUNS;
            double avg_merge = total_merge_times[size] / NUM_RUNS;
            double avg_heap = total_heap_times[size] / NUM_RUNS;

            AverageCaseResults << size << ","
                << avg_insertion << ","
                << avg_quick << ","
                << avg_merge << ","
                << avg_heap << '\n';
        }

        AverageCaseResults.close();
        std::cout << "Average case results saved to sorting_averagecase.csv" << std::endl;
    }

    std::cout << "All tests completed!" << std::endl;
    return 0;
}