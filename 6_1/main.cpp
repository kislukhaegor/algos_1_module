#include <iostream>

template<typename T>
size_t median(T* array, size_t left, size_t right) {
	T l = array[left];
	T r = array[right];
	size_t mid = (left + right) / 2;
	T m = array[mid];

	if (l > r) {
		if (m > l) {
			return left;
		}
		return (m > r) ? mid : right;
	}

	if (m > r) {
		return right;	
	}
	return (l > m) ? left : mid;
}

template<typename T>
size_t partition(T* array, size_t left, size_t right) {
	size_t pivot_i = median(array, left, right);
	std::swap(array[pivot_i], array[right]);
	size_t i = left;
	size_t j = left;
	while(j < right) {
		if (array[j] > array[right]) {
			++j;
		} else {
			std::swap(array[i++], array[j++]);
		}
	}
	std::swap(array[i], array[j]);
	return i;
}


template<typename T>
T get_k(T* array, size_t size, size_t k) {
	size_t left = 0;
	size_t right = size;
	while (true) {
		size_t pivot_i = partition(array, left, right - 1);
		if (pivot_i == k) {
			return array[pivot_i];
		} else if (k < pivot_i) {
			right = pivot_i;
		} else {
			left = pivot_i + 1;
		}
	}
}

int main() {
	size_t size = 0;
	size_t k = 0;
	std::cin >> size >> k;
	int* array = new int[size];
	for (size_t i = 0; i < size; ++i) {
		std::cin >> array[i];
	}
	std::cout << get_k<int>(array, size, k) << std::endl;
	delete [] array;
}