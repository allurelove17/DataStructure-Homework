#ifndef MERGE_HPP
#define MERGE_HPP
#include <iostream>
#include <algorithm>
#include <vector>
#include <chrono>

template<class T>
class Merge {
public:
    Merge(int SIZE) {
        array = new T[SIZE];
    }

    /*~Merge() {
        delete []array;
    }*/

    double mergetime(int left, int right) {
        auto start = std::chrono::high_resolution_clock::now();
        merge_sort(left, right);
        auto end = std::chrono::high_resolution_clock::now();
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

    void print(int size) {
        for (int i = 0; i < size; ++i) {
            std::cout << *(this->array + i) << ' ';
        }std::cout << '\n';
    }

    T* array;
private:
};
#endif