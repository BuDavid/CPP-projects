#include "vector.hpp"

template <typename T>
Vector<T>::Vector() noexcept : m_ptr(nullptr), m_size(0), m_capacity(0) {}

template <typename T>
Vector<T>::Vector(const std::initializer_list<T>& list) : m_size(0), m_capacity(1) {
    while (m_capacity <= list.size()) {
        m_capacity *= 2;
    }

    m_ptr = new value_type[m_capacity];
    for(const auto& elem : list) {
        m_ptr[m_size++] = elem;
    }
}

template <typename T>
Vector<T>::Vector(size_type count, const_reference value) : m_ptr(nullptr), m_size(count), m_capacity(0) {
    if (count == 0) {
        return;
    }

    m_capacity = 1;
    while (count >= m_capacity && (m_capacity *= 2)); 

    m_ptr = new value_type[m_capacity];
    for(size_type i = 0; i < m_size; i++) {
        m_ptr[i] = value;
    }
}

template <typename T>
Vector<T>::Vector(const Vector& other) {
	m_size = other.m_size;
	m_capacity = other.m_capacity;
	if (!other.m_ptr) {
		m_ptr = nullptr;
	} else {
        m_ptr = new value_type[m_capacity];
        for (size_type i = 0; i < m_size; i++) {
            m_ptr[i] = other.m_ptr[i];
        }
    }
}

template <typename T>
Vector<T>::Vector(Vector&& other) noexcept {
    m_size = std::exchange(other.m_size, 0);
    m_capacity = std::exchange(other.m_capacity, 0);
    m_ptr = std::exchange(other.m_ptr, nullptr);
}

template <typename T>
void Vector<T>::swap(Vector& other) noexcept {
    Vector<T> tmp(std::move(other));
    other = std::move(*this);
    *this = std::move(tmp); 
}

template <typename T>
Vector<T>& Vector<T>::operator=(const Vector& other) {
    if (this != &other) { 
        m_size = other.m_size;
        m_capacity = other.m_capacity;
        if(m_ptr) {
            delete [] m_ptr;
        }

        m_ptr = new value_type[m_capacity];
        for (size_type i = 0; i < m_size; i++) {
            m_ptr[i] = other.m_ptr[i];
        }
    }
    return *this;
}

template <typename T>
Vector<T>& Vector<T>::operator=(Vector&& other) noexcept {
    if (this != &other) {
        m_size = std::exchange(other.m_size, 0);
        m_capacity = std::exchange(other.m_capacity, 0);
        m_ptr = std::exchange(other.m_ptr, nullptr);
    }
    return *this;
}

template <typename T>
typename Vector<T>::Iterator Vector<T>::erase(const Iterator& pos) {
    if (pos < begin() || pos >= end()) {
        throw std::out_of_range("Position out of range");
    }

    size_type idx = pos - begin();

    for (size_type i = idx; i < m_size - 1; i++) {
        m_ptr[idx] = std::move(m_ptr[i + 1]);
    }
    m_size--;
    return begin() + idx;
}

template <typename T>
typename Vector<T>::Iterator Vector<T>::erase(const Iterator& first, const Iterator& last) {
    if (first < begin() || last > end() || first > last) {
        throw std::out_of_range("Position out of range");
    }
    size_type start = first - begin();
    size_type end = last - begin();
    size_type count = end - start;

    for (size_type i = end; i < m_size; i++) {
        m_ptr[start++] = std::move(m_ptr[i]);
    }
    m_size -= count;
    return begin() + start;
}

template <typename T>
typename Vector<T>::Iterator Vector<T>::insert(const Iterator& pos, const_reference value) {
    if (pos < begin() || pos > end()) {
        throw std::out_of_range("Position out of range");
    }

    if (m_size + 1 >= m_capacity) {
        resize(m_capacity * 2);
    }
    
    size_type idx = pos - begin();
    for (size_type i = m_size; i > idx; i--) {
        m_ptr[i] = std::move(m_ptr[i-1]);
    }

    m_ptr[idx] = value;
    m_size++;
    return begin() + idx;
}

template <typename T>
typename Vector<T>::Iterator Vector<T>::insert(const Iterator& pos, value_type&& value) {
    if (pos < begin() || pos > end()) {
        throw std::out_of_range("Position out of range");
    }

    if (m_size + 1 >= m_capacity) {
        resize(m_capacity * 2);
    }
    
    size_type idx = pos - begin();
    for (size_type i = m_size; i > idx; i--) {
        m_ptr[i] = std::move(m_ptr[i-1]);
    }

    m_ptr[idx] = std::move(value);
    m_size++;
    return begin() + idx;
}

template <typename T>
typename Vector<T>::Iterator Vector<T>::insert(const Iterator& pos, Iterator first, Iterator last) {
    if (pos < begin() || pos > end() || first >= last) {
        throw std::out_of_range("Position out of range");
    }

    size_type range_size = last - first;

    if (m_size + range_size >= m_capacity) {
        while (m_capacity <= m_size + range_size && (m_capacity *= 2));
        resize(m_capacity);
    }

    size_type idx = pos - begin();
    for (size_type i = m_size; i > idx; i--) {
        m_ptr[i + range_size] = std::move(m_ptr[i-1]);
    }

    while (first < last) {
        m_ptr[idx++] = *first;
        first++;
    }
    m_size += range_size;
    return begin() + idx;
}

template <typename T>
std::ostream& operator<<(std::ostream& os, const Vector<T>& other) {
	if (other.m_ptr) {
		for (typename Vector<T>::size_type i = 0; i < other.m_size; i++) {
			os << other.m_ptr[i] << ' ';
		}
		os << std::endl;
	}
	return os;
}

template <typename T>
bool Vector<T>::operator==(const Vector& other) const {
	if (m_size != other.m_size) {
		return 0;
	}
	for (size_type i = 0; i < m_size; i++) {
		if (m_ptr[i] != other.m_ptr[i]) {
			return 0;
		}
	}
	return 1;
}

template <typename T>
typename Vector<T>::reference Vector<T>::operator[](size_type idx) noexcept {
	return m_ptr[idx];
}

template <typename T>
typename Vector<T>::const_reference Vector<T>::operator[](size_type idx) const noexcept {
	return m_ptr[idx];
}

template <typename T>
typename Vector<T>::reference Vector<T>::at(size_type idx) {
	if (idx >= m_size) {
        throw std::out_of_range("Index is out of range.");
	}
	return m_ptr[idx];
}

template <typename T>
typename Vector<T>::const_reference Vector<T>::at(size_type idx) const {
	if (idx >= m_size) {
        throw std::out_of_range("Index is out of range.");
	}
	return m_ptr[idx];
}

template <typename T>
typename Vector<T>::reference Vector<T>::front() {
	return m_ptr[0];
}

template <typename T>
typename Vector<T>::const_reference Vector<T>::front() const {
	return m_ptr[0];
}

template <typename T>
typename Vector<T>::reference Vector<T>::back() {
	return m_ptr[m_size-1];
}

template <typename T>
typename Vector<T>::const_reference Vector<T>::back() const {
	return m_ptr[m_size-1];
}

template <typename T>
void Vector<T>::push_back(const_reference element) {
	if (m_size + 1 >= m_capacity) {
        reserve(m_capacity ? m_capacity * 2 : 1);
	}
	m_ptr[m_size++] = element;
}

template <typename T>
void Vector<T>::push_back(value_type&& element) {
	if (m_size + 1 >= m_capacity) {
        reserve(m_capacity ? m_capacity * 2 : 1);
	}
	m_ptr[m_size++] = std::move(element);
}

template <typename T>
void Vector<T>::pop_back() {
	if (m_size && m_size--); 
}

template <typename T>
bool Vector<T>::empty() const {
	return !m_size;
}

template <typename T>
typename Vector<T>::size_type Vector<T>::size() const {
	return m_size;
}

template <typename T>
typename Vector<T>::size_type Vector<T>::max_size() const {
	return std::numeric_limits<size_type>::max() / sizeof(value_type);
}

template <typename T>
typename Vector<T>::size_type Vector<T>::capacity() const {
	return m_capacity;
}

template <typename T>
void Vector<T>::clear() {
    m_size = 0;
}

template <typename T>
void Vector<T>::shrink_to_fit() {
	if (m_size < m_capacity) {
        reserve(m_size);
	}
}

template <typename T>
void Vector<T>::resize(size_type count) {
    size_type tmp = m_capacity ? m_capacity : 1;
    if (count >= tmp) {
        while (count >= tmp && (tmp *= 2));
        reserve(tmp);
    }
    m_size = count;
}

template <typename T>
void Vector<T>::resize(size_type count, const_reference value) {
    size_type tmp = m_capacity ? m_capacity : 1;
    if (count >= tmp) {
        while (count >= tmp && (tmp *= 2));
        reserve(tmp);
    }

    for (size_type i = m_size; i < count; i++) {
        m_ptr[i] = value;
    }
    m_size = count;
}

template <typename T>
void Vector<T>::reserve(size_type new_cap) {
    if (new_cap > max_size()) {
        throw std::invalid_argument("The size can't be negative.");
    }

    if (m_capacity > new_cap) {
        return;
    }

    m_capacity = new_cap;

	pointer tmp = m_ptr;
	m_ptr = new value_type[m_capacity];
	for (size_type i = 0; i < m_size; i++) {
		m_ptr[i] = std::move(tmp[i]);
	}

	delete [] tmp;
	tmp = nullptr;
}

template <typename T>
typename Vector<T>::ConstIterator Vector<T>::begin() const {
    return ConstIterator(m_ptr);
}

template <typename T>
typename Vector<T>::Iterator Vector<T>::begin() {
    return Iterator(m_ptr);
}

template <typename T>
typename Vector<T>::ConstIterator Vector<T>::end() const {
    return ConstIterator(m_ptr + m_size);
}

template <typename T>
typename Vector<T>::Iterator Vector<T>::end() {
    return Iterator(m_ptr + m_size);
}

template <typename T>
typename Vector<T>::ConstIterator Vector<T>::cbegin() const noexcept {
    return ConstIterator(m_ptr);
}

template <typename T>
typename Vector<T>::ConstIterator Vector<T>::cend() const noexcept {
    return ConstIterator(m_ptr + m_size);
}

template <typename T>
typename Vector<T>::CReverseIterator Vector<T>::rbegin() const {
    return CReverseIterator(m_ptr + m_size);
}

template <typename T>
typename Vector<T>::ReverseIterator Vector<T>::rbegin() {
    return ReverseIterator(m_ptr + m_size);
}

template <typename T>
typename Vector<T>::CReverseIterator Vector<T>::rend() const {
    return CReverseIterator(m_ptr);
}

template <typename T>
typename Vector<T>::ReverseIterator Vector<T>::rend() {
    return ReverseIterator(m_ptr);
}

template <typename T>
typename Vector<T>::CReverseIterator Vector<T>::crbegin() const noexcept {
    return CReverseIterator(m_ptr + m_size);
}

template <typename T>
typename Vector<T>::CReverseIterator Vector<T>::crend() const noexcept {
    return CReverseIterator(m_ptr);
}

template <typename T>
typename Vector<T>::pointer Vector<T>::data() {
    return m_ptr;
}

template <typename T>
typename Vector<T>::const_pointer Vector<T>::data() const {
    return m_ptr;
}

template <typename T>
Vector<T>::~Vector() {
	m_size = 0;
	m_capacity = 0;
	if (m_ptr) {
		delete [] m_ptr;
	}
	m_ptr = nullptr;
}
