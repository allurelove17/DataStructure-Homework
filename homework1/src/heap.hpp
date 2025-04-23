#ifndef HEAP_HPP
#define HEAP_HPP

#include <iostream>
#include <algorithm>
#include <vector>
#include <chrono>

template<class T>
class heap {
public:
	heap(int SIZE) {
		array = new T[SIZE + 1];
	}
	
	void maxheap(int root, int length) {
		int left = 2 * root, right = 2 * root + 1, max;
		T c;
		if (left <= length && *(this->array + left) > *(this->array + root))
			max = left;
		else
			max = root;
		if (right <= length && *(this->array + right) > *(this->array + max))
			max = right;
		if (max != root) {
			c = *(this->array + root);
			*(this->array + root) = *(this->array + max);
			*(this->array + max) = c;
			maxheap(max, length);
		}
	}
	
	void build(int size) {
		for (int i = size / 2; i >= 1; --i) {
			maxheap(i, size - 1);
		}
	}

	double heap_sort(int size) {
		auto start = std::chrono::steady_clock::now();
		T c;
		//*(this->array) = 0;
		for (int i = size + 1; i > 0; --i) {
			*(this->array + i) = *(this->array + i - 1);
		}
		*(this->array) = 0;

		build(size);

		int len = size;
		for (int i = size; i >= 2; --i) {
			c = *(this->array + 1);
			*(this->array + 1) = *(this->array + i);
			*(this->array + i) = c;
			--len;
			maxheap(1, len);
		}
		for (int i = 0; i < size; i++) {
			*(this->array + i) = *(this->array + i + 1);
		}
		auto end = std::chrono::steady_clock::now();
		return std::chrono::duration<double, std::milli>(end - start).count();
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
