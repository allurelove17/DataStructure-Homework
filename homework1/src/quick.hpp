#ifndef QUICK_HPP
#define QUICK_HPP

#include <iostream>
#include <algorithm>
#include <chrono>

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
				//swap((this->array + i), (this->array + j));
				c = *(this->array + i);
				*(this->array + i) = *(this->array + j);
				*(this->array + j) = c;

			}
			++j;
		}
		++i;
		//swap((this->array + i), (this->array + right));
		c = *(this->array + i);
		*(this->array + i) = *(this->array + right);
		*(this->array + right) = c;
		return i;
	}
	//void swap(T&, T&);
	void quick_sort(int left, int right) {
		/*if (left < right) {
			int p = pivot(left, right);
			quick_sort(left, p - 1);
			quick_sort(p + 1, right);
		}*/
		while (left < right) {
			int p = pivot(left, right);
			if (p - left <= right - p ) {
				quick_sort(left, p - 1);
				left = p + 1;
			}
			else {
				quick_sort(p + 1, right);
				right = p - 1;
			}
		}
	}

	double quicktime(int left, int right) {
		auto start = std::chrono::steady_clock::now();
		quick_sort(left, right);
		auto end = std::chrono::steady_clock::now();
		return std::chrono::duration<double, std::milli>(end - start).count();
	}

	T* array;
private:

};

#endif
