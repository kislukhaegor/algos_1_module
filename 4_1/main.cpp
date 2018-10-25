#include <iostream>
#include <cassert>
#include <cstring>

/*
Вовочка ест фрукты из бабушкиной корзины.
В корзине лежат фрукты разной массы. Вовочка может поднять не более K грамм. Каждый фрукт весит не более K грамм. За раз он выбирает несколько самых тяжелых фруктов, которые может поднять одновременно, откусывает от каждого половину и кладет огрызки обратно в корзину. Если фрукт весит нечетное число грамм, он откусывает большую половину. Фрукт массы 1гр он съедает полностью.

Определить за сколько подходов Вовочка съест все фрукты в корзине.
*/


template<typename T>
T* cpy_non_pod(T* dest, const T* src, size_t n) {
	for (size_t i = 0; i < n; ++i) {
		dest[i] = src[i];
	}
	return dest;
}

template<typename T, size_t AllocSize = 8>
class Array {
public:
	Array();
	Array(T*, size_t);
	Array(const Array&);
	Array<T, AllocSize>& operator=(const Array<T, AllocSize>&);
	Array(Array&&);
	Array<T, AllocSize>&& operator=(Array<T, AllocSize>&&);
	~Array();

	T& operator[](size_t i);
	const T& operator[](size_t i) const;
	size_t size() const;
	void push_back(const T&);
	T pop_back();
private:
	void realloc();

	T* data;
	size_t size_;
	size_t alloc_size;
};


template<typename T>
class Heap {
public:
	Heap() = default;
	Heap(T*, size_t); 	
	void append(const T&);
	T extractMax();
	T readMax() const;
	size_t size() const;
private:
	void shift_down(size_t);
	void shift_up(int);

	Array<T> array;
};

template<typename T, size_t AllocSize>
Array<T, AllocSize>::Array(T* data, size_t size)
: data(new T[std::max(size, AllocSize)]), alloc_size(std::max(size, AllocSize)), size_(size) {
	if (std::is_pod<T>::value) {
		memcpy(this->data, data, size * sizeof(T));
	} else {
		cpy_non_pod(this->data, data, size);
	}
}

template<typename T>
Heap<T>::Heap(T* data, size_t size)
: array(data, size) {
	for (int i = size / 2 - 1; i >= 0; --i) {
		shift_down(i);
	}
}

template<typename T, size_t AllocSize>
Array<T, AllocSize>::Array()
: data(new T[AllocSize]), size_(0), alloc_size(AllocSize)
{
}

template<typename T, size_t AllocSize>
Array<T, AllocSize>::Array(const Array<T, AllocSize>& other)
: data(new T[other.alloc_size]), size_(other.size_), alloc_size(other.alloc_size)
{
	if (std::is_pod<T>::value) {
		memcpy(this->data, data, size_ * sizeof(T));
	} else {
		cpy_non_pod(this->data, data, size_);
	}
}

template<typename T, size_t AllocSize>
Array<T, AllocSize>::Array(Array<T, AllocSize>&& other)
: data(std::move(other.data)), size_(std::move(other.size_)), alloc_size(std::move(other.alloc_size)) {
	other.data = nullptr;
	other.size_ = 0;
	other.alloc_size = 0;
}

template<typename T, size_t AllocSize>
Array<T, AllocSize>& Array<T, AllocSize>::operator=(const Array<T, AllocSize>& other) {
	if (this != &other) {
		delete [] data;
		data = new T[other.alloc_size];
		if (std::is_pod<T>::value) {
			memcpy(data, other.data, other.alloc_size * sizeof(T));
		} else {
			cpy_non_pod(data, other.data, other.alloc_size);
		}
		size_ = other.size_;
		alloc_size = other.alloc_size;
	}
}

template<typename T, size_t AllocSize>
Array<T, AllocSize>&& Array<T, AllocSize>::operator=(Array<T, AllocSize>&& other) {
	if (this != &other) {
		delete [] data;
		data = std::move(other.data);
		size_ = std::move(other.size_);
		alloc_size = std::move(other.alloc_size);
		other.data = nullptr;
		other.size_ = 0;
		other.alloc_size = 0;
	}
}

template<typename T, size_t AllocSize>
Array<T, AllocSize>::~Array() {
	delete [] data;
}

template<typename T, size_t AllocSize>
T& Array<T, AllocSize>::operator[](size_t i) {
	assert(i < size_);
	return data[i];
}

template<typename T, size_t AllocSize>
const T& Array<T, AllocSize>::operator[](size_t i) const {
	assert(i < size_);
	return data[i];
}

template<typename T, size_t AllocSize>
size_t Array<T, AllocSize>::size() const {
	return size_;
}

template<typename T, size_t AllocSize>
void Array<T, AllocSize>::push_back(const T& elem) {
	if (size_ == alloc_size) {
		realloc();
	}
	data[size_++] = elem;
}

template<typename T, size_t AllocSize>
void Array<T, AllocSize>::realloc() {
	alloc_size *= 2;
	T* new_data = new T[alloc_size];
	if (std::is_pod<T>::value) {
		memcpy(new_data, data, size_ * sizeof(T));
	} else {
		cpy_non_pod(new_data, data, size_);
	}
	delete[] data;
	data = new_data;
}

template<typename T, size_t AllocSize>
T Array<T, AllocSize>::pop_back() {
	return data[--size_];
}

template<typename T>
void Heap<T>::shift_down(size_t i) {
	size_t left = 2 * i + 1;
	size_t right = 2 * i + 2;
	size_t largest = i;
	if (left < array.size() && array[left] > array[largest]) {
		largest = left;
	}
	if (right < array.size() && array[right] > array[largest]) {
		largest = right;
	}
	if (largest != i) {
		std::swap(array[i], array[largest]);
		shift_down(largest);
	}
}

template<typename T>
void Heap<T>::shift_up(int index) {
	while (index > 0) {
		int parent = (index - 1) / 2;
		if (array[index] <= array[parent]) {
			return;
		}
		std::swap(array[index], array[parent]);
		index = parent;
	}
}

template<typename T>
void Heap<T>::append(const T& elem) {
	array.push_back(elem);
	shift_up(array.size() - 1);
}

template<typename T>
T Heap<T>::extractMax() {
	assert(array.size() != 0);
	T result = array[0];
	array[0] = array.pop_back();
	if (array.size() != 0) {
		shift_down(0);
	}
	return result;
}

template<typename T>
T Heap<T>::readMax() const {
	assert(array.size() != 0);
	return array[0];
}

template<typename T>
size_t Heap<T>::size() const {
	return array.size();
}

unsigned take_fruits(Heap<int>& heap, unsigned max_weigt) {
	unsigned* buffer = new unsigned[max_weigt];
	int count = 0;
	unsigned sum_weight = 0;
	while (heap.size() && (sum_weight + heap.readMax() <= max_weigt)) {
		sum_weight += heap.readMax();
		buffer[count++] = heap.extractMax();
	}
	for (int i = 0; i < count; ++i) {
		unsigned tmp = buffer[i] / 2;
		if (tmp) {
			heap.append(tmp);
		}
	}
	++count;
	delete [] buffer;
}

unsigned count_taking(Heap<int>& heap, int k) {
	unsigned count = 0;
	while(heap.size()) {
		take_fruits(heap, k);
		++count;
	}
	return count;
}

int main() {
	int n = 0;
	std::cin >> n;
	int* buffer = new int[n];
	int size = 0;
	for (int i = 0; i < n; ++i) {
		unsigned tmp = 0;
		std::cin >> tmp;
		if (tmp > 0) {
			buffer[size] = tmp;
			++size;
		}
	}
	Heap<int> heap(buffer, size);
	delete [] buffer;	
	int k = 0;
	std::cin >> k;
	std::cout << count_taking(heap, k) << std::endl;
	return 0;
}
