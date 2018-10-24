#include <iostream>
#include <cmath>
#include <cassert>


template<typename T>
T* binary_search(T* begin, T* end, const T& elem) {
	while (begin < end) {
		size_t middle = (end - begin) / 2;
		if (begin[middle] < elem) {
			begin = begin + middle + 1;
		} else {
			end = begin + middle;
		}
	}
	return begin;
}

template<typename T>
T* optymized_binary_search(T* begin, T* end, const T& elem) {
	size_t first = 0;
	for (std::ptrdiff_t last = 1; last < end - begin; last *= 2) {
		if (elem < begin[last]) {
			return binary_search<T>(begin + first, begin + last, elem);
		}
		first = last;
	}
	return binary_search<T>(begin + first, end, elem);
}

template<typename T>
size_t intersection(T* first_mass,
                    size_t first_mass_size,
                    T* second_mass,
                    size_t second_mass_size,
                    T* result_mass)
{
	size_t res_i = 0;
	T* cur_begin = first_mass;
	T* end = first_mass + first_mass_size;
	for (size_t i = 0; i < second_mass_size; ++i) {
		T* tmp = optymized_binary_search<T>(cur_begin,
		                                    end,
		                                    second_mass[i]);
		if (tmp == end) {
			return res_i;
		}
		if (*tmp == second_mass[i]) {
			result_mass[res_i++] = *tmp;
			cur_begin = tmp + 1;
		} else {
			tmp = tmp > cur_begin ? tmp - 1 : cur_begin;
		}
	}
	return res_i;
}

int main() {
	int n = 0;
	std::cin >> n;
	int k = 0;
	std::cin >> k;
	assert(n > k);
	
	int* first_mass = new int[n];
	int* second_mass = new int[k];
	
	for (int i = 0; i < n; ++i) {
		std::cin >> first_mass[i];
	}
	for (int i = 0; i < k; ++i) {
		std::cin >> second_mass[i];
	}

	int* result_mass = new int[k];
	size_t size = intersection(first_mass, n, second_mass, k, result_mass);
	for (size_t i = 0; i < size; ++i) {
		std::cout << result_mass[i] << " ";
	}
	std::cout << std::endl;
}
