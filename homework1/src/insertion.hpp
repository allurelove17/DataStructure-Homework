#ifndef INSERTION_HPP
#define INSERTION_HPP

template<class T>
class insertion{
public:
	insertion(int SIZE) {
		array = new T[SIZE];
	}

	~insertion() {
		delete []array;
	}

	double insertion_sort(int size) {
		auto start = std::chrono::high_resolution_clock::now();
		for (int i = 1; i < size; ++i) {
			int value = *(this->array + i), j;
			for (j = i - 1; j >= 0 && value < *(this->array + j); --j) {
				if (value < *(this->array + j))
					*(this->array + j + 1) = *(this->array + j);
			}
			*(this->array + j + 1) = value;
		}
		auto end = std::chrono::high_resolution_clock::now();
		// return std::chrono::duration<double, std::micro>(end - start).count();
		return std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
		// duration_cast: time type switch -f
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
