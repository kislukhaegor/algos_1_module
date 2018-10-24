#include <iostream>
#include <string.h>
#include <vector>
#include <cassert>

/*
Дан массив строк. Количество строк не больше 100000. Отсортировать массив методом поразрядной сортировки MSD по символам. Размер алфавита - 256 символов. Последний символ строки = ‘
0’.
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
	Array(size_t);
	Array(const T*, size_t);
	Array(const Array&);
	Array& operator=(const Array&);
	Array(Array&&);
	Array&& operator=(Array&&);
	~Array();

	void clear();
	T& operator[](size_t);
	const T& operator[](size_t) const;
	const T* get_data() const;
	size_t get_alloc_size() const;
	size_t size() const;
	void push_back(const T&);
	T pop_back();
private:
	void realloc();
	T* data;
	size_t size_;
	size_t alloc_size;
};



template<typename T, size_t AllocSize>
Array<T, AllocSize>::Array()
: data(new T[AllocSize]), size_(0), alloc_size(AllocSize)
{
	if (std::is_pod<T>::value) {
		memset(data, 0, alloc_size * sizeof(T));
	} 
}

template<typename T, size_t AllocSize>
Array<T, AllocSize>::Array(size_t size)
: data(new T[size]), size_(size), alloc_size(size)
{
	if (std::is_pod<T>::value) {
		memset(data, 0, alloc_size * sizeof(T));
	}
}

template<typename T, size_t AllocSize>
Array<T, AllocSize>::Array(const T* data, size_t size)
: data(new T[std::max(size, AllocSize)]), size_(size), alloc_size(std::max(size, AllocSize)) {
	if (std::is_pod<T>::value) {
		memcpy(this->data, data, size * sizeof(T));
	} else {
		cpy_non_pod(this->data, data, size);
	}
}

template<typename T, size_t AllocSize>
Array<T, AllocSize>::Array(const Array<T, AllocSize>& other)
: Array<T, AllocSize>(other.data, other.size_)
{
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
Array<T, AllocSize>::Array(Array<T, AllocSize>&& other)
: data(std::move(other.data)), size_(std::move(other.size_)), alloc_size(std::move(other.alloc_size)) {
	other.data = nullptr;
	other.size_ = 0;
	other.alloc_size = 0;
}

template<typename T, size_t AllocSize>
Array<T, AllocSize>&& Array<T, AllocSize>::operator=(Array<T, AllocSize>&& other) {
	if (this != &other) {
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
void Array<T, AllocSize>::clear() {
	if (!std::is_pod<T>::value) {
		for (size_t i = 0; i < size_; ++i) {
			data[i] = T();
		}
	}
	memset(data, 0, size_ * sizeof(T));
	size_ = 0;
}

template<typename T, size_t AllocSize>
T& Array<T, AllocSize>::operator[](size_t i) {
	if (i >= size_) {
		std::cout << i << " " << size_ << std::endl;
	}
	assert(i < size_);
	return data[i];
}

template<typename T, size_t AllocSize>
const T& Array<T, AllocSize>::operator[](size_t i) const {
	assert(i < size_);
	return data[i];
}
template<typename T, size_t AllocSize>
const T* Array<T, AllocSize>::get_data() const {
	return data;	
}

template<typename T, size_t AllocSize>
size_t Array<T, AllocSize>::get_alloc_size() const {
	return alloc_size;
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
T Array<T, AllocSize>::pop_back() {
	assert(size_ != 0);
	T tmp = std::move(data[size_]);
	size--;
	return tmp;
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

class String : public Array<char> {
public:
	String();
	String(const char* c_str);
	size_t len() const;  // возвращает длину без нулевого байта
	friend std::istream& operator>>(std::istream&, String&);

};

String::String()
: Array<char>() {
	push_back(0);
}

String::String(const char* c_str)
: Array<char>(c_str, strlen(c_str)) {
}


size_t String::len() const {
	return size() - 1;
}

std::istream& operator>>(std::istream& is, String& str) {
	str.clear();
	char c = 0;
	while (is.get(c) && c != '\n') {
		str.push_back(c);
	}
	str.push_back('\0');
	return is;
}

std::ostream& operator<<(std::ostream& os, String& str) {
	return os << str.get_data();
}

void counting_sort(Array<String>& arr, int left, int right, int index, int* counts) {
	for (int i = left; i < right; ++i) {
		counts[arr[i][index]]++;
	}
	for (int i = 1; i < 256; ++i) {
		counts[i] += counts[i - 1]; 
	}
	Array<String> buffer(right - left);
	for (int i = right - 1; i >= left; --i) {
		buffer[--counts[arr[i][index]]] = arr[i];
	}
	for (int i = left; i < right; ++i) {
		arr[i] = buffer[i - left];
	}
}


void msd_sort(Array<String>& arr, size_t left, size_t right, size_t index) {
	if (right - left <= 1) {
		return;
	}

	int counts[257] = {};
	counts[256] = right - left;
	counting_sort(arr, left, right, index, counts);
	for (size_t i = 1; i < 256; ++i) {
		if (counts[i + 1] - counts[i] > 1) {
			msd_sort(arr, counts[i] + left, counts[i + 1] + left, index + 1);
		}
	}
}

int main() {
	Array<String> arr;
	String str;
	while (std::cin >> str) {
		arr.push_back(str);
	}
	msd_sort(arr, 0, arr.size(), 0);
	for (size_t i = 0; i < arr.size(); ++i) {
		std::cout << arr[i] << std::endl;
	}
	return 0;
}