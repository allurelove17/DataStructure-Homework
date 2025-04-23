#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

template<class T>
class Worst_Permutation {
public:
	//void permutation(int);
	Worst_Permutation(int n) {
		/*srand(time(NULL));
		std::ofstream ofs;
		ofs.open("test.txt");
		if (!ofs.is_open()) {
			std::cout << "Failed to open file!\n";
		}
		for (int i = 0; i < n; ++i) {
			int x = rand() % INT_MAX;
			ofs << x << ' ';
		}
		ofs.close();
		*/
		std::ofstream WorstCaseOfInsertion, WorstCaseOfMerge;
		WorstCaseOfInsertion.open("worstinser.txt");
		if (!WorstCaseOfInsertion.is_open()) {
			std::cout << "worstinser.txt failed to open!\n";
		}
		for (int i = 0; i < n; ++i) {
			WorstCaseOfInsertion << n - i << ' ';
		}
		WorstCaseOfInsertion.close();

		WorstCaseOfMerge.open("worstmerge.txt");
		if(!WorstCaseOfMerge.is_open()) {
			std::cout << "worstmerge.txt failed to open!\n";
		}
		std::vector<T> worstcaseOfmerge = worstCaseOfmerge(n);
		for (auto x : worstcaseOfmerge) {
			WorstCaseOfMerge << x << ' ';
		}
		WorstCaseOfMerge.close();
	}
private:
	std::vector<T> worstCaseOfmerge(int n) {
		if (n == 1) {
			return { 1 };
		}
		else {
			int topsize = std::floor(static_cast<float>(n) / 2);
			int bottomsize = std::ceil(static_cast<float>(n) / 2);

			std::vector<T> top = worstCaseOfmerge(topsize);
			std::vector<T> bottom = worstCaseOfmerge(bottomsize);

			std::vector<T> result;
			result.reserve(n);

			for (int i = 0; i < top.size(); ++i) {
				result.push_back(top[i] * 2);
			}
			for (int i = 0; i < bottom.size(); ++i) {
				result.push_back(bottom[i] * 2 - 1);
			}
			return result;
		}	
	}
};