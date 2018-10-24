#include <type_traits>
#include <iostream>
#include <cstring>

template<typename T>
T* cpy_non_pod(T* dest, const T* src, size_t n) {
	for (size_t i = 0; i < n; ++i) {
		dest[i] = src[i];
	}
	return dest;
}


template<typename T, size_t AllocSize = 128>
class CycleBuffer {
public:
	CycleBuffer();
	~CycleBuffer();
	CycleBuffer(CycleBuffer&) = delete;
	CycleBuffer(CycleBuffer&&) = delete;
	CycleBuffer& operator=(CycleBuffer&) = delete;
	CycleBuffer&& operator=(CycleBuffer&&) = delete;
	T pop();
	void push_back(const T&);
private:
	void realloc();

	T* data;
	size_t offset;
	size_t size_;
	size_t alloc_size;
};

template<typename T, size_t AllocSize>
CycleBuffer<T, AllocSize>::CycleBuffer()
: data(new T[AllocSize]), offset(0), size_(0), alloc_size(AllocSize)
{
}

template<typename T, size_t AllocSize>
void CycleBuffer<T, AllocSize>::realloc() {
	T* new_data = new T[alloc_size * 2];
	if (std::is_pod<T>::value) {
		memcpy(new_data, data + offset, (alloc_size - offset) * sizeof(T));
		memcpy(new_data + alloc_size - offset, data, offset * sizeof(T));
	} else {
		cpy_non_pod(new_data, data + offset, (alloc_size - offset));
		cpy_non_pod(new_data + alloc_size - offset, data, offset);
	}
	alloc_size *= 2;
	offset = 0;
	delete [] data;
	data = new_data;
}

template<typename T, size_t AllocSize>
CycleBuffer<T, AllocSize>::~CycleBuffer() {
	delete [] data;
}

template<typename T, size_t AllocSize>
void CycleBuffer<T, AllocSize>::push_back(const T& elem) {
	if (size_ >= alloc_size) {
		realloc();
	}
	data[(offset + size_) % alloc_size] = elem;
	++size_;
}

template<typename T, size_t AllocSize>
T CycleBuffer<T, AllocSize>::pop() {
	if (size_ == 0) {
		return T(-1);
	}
	--size_;
	T tmp = data[offset];
	offset = (offset + 1) % alloc_size;
	return tmp;
}

int main() {
	CycleBuffer<int> queue;
	int n = 0;
	bool ok = true;
	std::cin >> n;
	for (int i = 0; i < n; ++i) {
		int a = 0;
		int b = 0;
		std::cin >> a >> b;
		switch (a) {
			case 2: {
				int tmp = queue.pop();
				if (tmp != b) {
					ok = false;
				}
				break;
			}
			case 3: {
				queue.push_back(b);
				break;
			}
			default:
				break;
		}
	}
	std::cout << (ok ? "YES" : "NO") << std::endl; 
	return 0;
}
