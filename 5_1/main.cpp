#include <string.h>
#include <iostream>
#include <memory>
#include <vector>
#include <algorithm>

/*
В супермаркете решили оптимизировать показ рекламы. Известно расписание прихода и ухода покупателей (два целых числа). Каждому покупателю необходимо показать минимум 2 рекламы. Рекламу можно транслировать только в целочисленные моменты времени. Покупатель может видеть рекламу от момента прихода до момента ухода из магазина. В каждый момент времени может показываться только одна реклама. Считается, что реклама показывается мгновенно. Если реклама показывается в момент ухода или прихода, то считается, что посетитель успел её посмотреть. 
Требуется определить минимальное число показов рекламы.
*/

template<typename T>
class Less {
public:
	constexpr bool operator()(const T &lhs, const T &rhs) const {
	    return lhs < rhs;
	}
};


class CustomerLine {
public:
	CustomerLine(int left = 0, int right = 0);
	void view_add();
	int get_ads() const;
	int get_left() const;
	int get_right() const;
	friend std::istream& operator>>(std::istream&, CustomerLine&);
private:
	int left;
	int right;
	int ads_count;
};

bool operator<(const CustomerLine& lhs, const CustomerLine& rhs) {
	if (lhs.get_right() < rhs.get_right()) {
		return true;
	} else if (lhs.get_right() == rhs.get_right()) {
		return lhs.get_left() > rhs.get_left();
	}
	return false;
}

CustomerLine::CustomerLine(int left, int right)
: ads_count(0)
{
	if (left > right) {
		this->left = right;
		this->right = left;
	}
}

void CustomerLine::view_add() {
	++ads_count;
}

int CustomerLine::get_ads() const {
	return ads_count;
}

int CustomerLine::get_left() const {
	return left;
}

int CustomerLine::get_right() const {
	return right;
}

std::istream& operator>>(std::istream& is, CustomerLine& l) {
	is >> l.left >> l.right;
	if (l.left > l.right) {
		std::swap(l.left, l.right);
	}
}


template<typename T, typename Comp = Less<T>>
void merge(T* array, size_t left, size_t right, size_t middle, T* buffer, Comp comp = Comp()) {
	size_t cur_l = 0;
	size_t cur_r = 0;
	while (left + cur_l < middle && middle + cur_r < right) {
		if (comp(array[left + cur_l], array[middle + cur_r])) {
			buffer[cur_l + cur_r] = array[left + cur_l];
			cur_l++;
		} else {
			buffer[cur_l + cur_r] = array[middle + cur_r];
			cur_r++;
		}
	}
	while (left + cur_l < middle) {
		buffer[cur_l + cur_r] = array[left + cur_l];
		cur_l++;
	}
	while (middle + cur_r < right) {
		buffer[cur_l + cur_r] = array[middle+ cur_r];
		cur_r++;
	}
	for (size_t i = 0; i < cur_r + cur_l; ++i) {
		array[left + i] = buffer[i];
	}
}

template<typename T, typename Comp = Less<T>>
void merge_sort(T* array, size_t size, Comp comp = Comp()) {
	T* buffer = new T[size];
	for (size_t i = 1; i < size; i *= 2) {
		for (size_t j = 0; j < size - i; j += i * 2) {
			merge(array, j, std::min(j + i * 2, size), j + i, buffer);
		}
	}
}

void show_add(CustomerLine* array, int start, int right_time, size_t size) {
	for (int i = start; i < size && array[i].get_left() <= right_time; ++i) {
		array[i].view_add();
	}
}

int count_ads(CustomerLine* array, size_t size) {
	int ads_count = 0;
	for (int i = 0; i < size; ++i) {
		if (array[i].get_ads() == 0) {
			show_add(array, i, array[i].get_right() - 1, size);
			++ads_count;
		}
		if (array[i].get_ads() == 1) {
			show_add(array, i, array[i].get_right(), size);
			++ads_count;
		}
	}
	return ads_count;
}


int main() {
	size_t size = 0;
	std::cin >> size;
	if (!size) {
		return 0;
	}

	CustomerLine* array = new CustomerLine[size];
	for (size_t i = 0; i < size; ++i) {
		std::cin >> array[i];
	}
	merge_sort(array, size);
	std::cout << count_ads(array, size) << std::endl;
	delete [] array;
}
