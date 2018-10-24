#include <iostream>
#include <type_traits>

#include <string.h>

template<typename T, size_t AllocSize = 16>
class RoundBuffer {
	static_assert(std::is_pod<T>::value, "T must be POD");
public:
	RoundBuffer();
	~RoundBuffer();
	void push_back(const T&);
	T pop(size_t num);
	T pop();
	T operator[](int i) const;
	T& operator[](int i);
	size_t size() const;
private:
	void realloc_buf();
	T* data;
	size_t size_;
	size_t alloc_size;
};

template<typename T, size_t AllocSize>
RoundBuffer<T, AllocSize>::RoundBuffer()
	: data(new T[AllocSize]), size_(0), alloc_size(AllocSize)
{
}

template<typename T, size_t AllocSize>
RoundBuffer<T, AllocSize>::~RoundBuffer() {
	delete [] data;
}

template<typename T, size_t AllocSize>
void RoundBuffer<T, AllocSize>::push_back(const T& elem) {
	if (size_ == alloc_size) {
		realloc_buf();
	}
	data[size_++] = elem;
}

template<typename T, size_t AllocSize>
T RoundBuffer<T, AllocSize>::pop(size_t num) {
	T tmp = data[num];
	num %= size_;
	if (num == size_ - 1) {
		return pop();
	}
	memmove(data + num, data + num + 1, (size_ - num + 1) * sizeof(T));
	--size_;
}

template<typename T, size_t AllocSize>
T RoundBuffer<T, AllocSize>::pop() {
	--size_;
	return data[size_];
}

template<typename T, size_t AllocSize>
void RoundBuffer<T, AllocSize>::realloc_buf() {
	T* tmp_data = data;
	alloc_size *= 2;
	data = new T[alloc_size];
	memmove(data, tmp_data, size_ * sizeof(T));
	delete [] tmp_data;
}

template<typename T, size_t AllocSize>
T RoundBuffer<T, AllocSize>::operator[](int i) const {
	return data[i % size_];
}

template<typename T, size_t AllocSize>
T& RoundBuffer<T, AllocSize>::operator[](int i) {
	return data[i % size_];
}

template<typename T, size_t AllocSize>
size_t RoundBuffer<T, AllocSize>::size() const {
	return size_;
}

int main() {
	int n = 0;
	int k = 0;
	std::cin >> n >> k;
	RoundBuffer<int> rb;
	for (int i = 1; i <= n; ++i) {
		rb.push_back(i);
	}
	int num = 0;
	while (rb.size() != 1) {
		num += k - 1;
		num %= rb.size();
		rb.pop(num);
	}
	std::cout << rb[0] << std::endl;
}