//
// Created by Nikita Shabanov on 01.12.2025.
//
#include <cstdlib>
#include "array.h"

#include <algorithm>
#include <ios>
#include <iterator>
#include <ranges>
#include <type_traits>
#include <vector>
#include <bits/ios_base.h>

#include "google_tests/lib/googlemock/include/gmock/internal/gmock-internal-utils.h"

template<typename T>
Array<T>::Array() : capacity(8), typeSize(sizeof(T))  {

}

template<typename T>
Array<T>::Array(int capacity) : capacity(capacity), typeSize(sizeof(T)) {
    data = static_cast<T*>(malloc(capacity * typeSize));
}

template<typename T>
Array<T>::~Array() {
    for (auto i = 0; i < currCount; i++) {
        data[i].~T();
    }
    free(data);
}

template<typename T>
int Array<T>::size() const {
    return currCount;
}

template<typename T>
int Array<T>::insert(const T &value) {
    if (currCount >= capacity) {
        capacity *= 2;
        auto relocData = static_cast<T*>(malloc(capacity * typeSize));
        if (std::is_move_constructible<T>::value == true) {
            for (auto i = 0; i < currCount; i++) {
                new (&relocData[i]) T(std::move(data[i]));
            }
        }
        else {
            for (auto i = 0; i < currCount; i++) {
                new (&relocData[i]) T(data[i]);
            }
        }

        new (&relocData[currCount]) T(value);

        for (auto i = 0; i < currCount; i++) {
            data[i].~T();
        }
        free(data);

        data = relocData;
        currCount++;
    }
    else
        new (&data[currCount++]) T(value);
    return currCount - 1;
}

template<typename T>
int Array<T>::insert(int index, const T& value) {
    if (index < 0 || index > currCount)
        throw std::out_of_range("index");

    if (currCount >= capacity) {
        capacity *= 2;
        auto relocData = static_cast<T*>(malloc(capacity * typeSize));
        if (std::is_move_constructible<T>::value == true) {
            for (auto i = 0; i < index; i++) {
                new (&relocData[i]) T(std::move(data[i]));
            }
            new (&relocData[index]) T(value);
            for (auto i = index; i < currCount; i++) {
                new (&relocData[i + 1]) T(std::move(data[i]));
            }
        }
        else {
            for (auto i = 0; i < index; i++) {
                new (&relocData[i]) T(data[i]);
            }
            new (&relocData[index]) T(value);
            for (auto i = index; i < currCount; i++) {
                new (&relocData[i + 1]) T(data[i]);
            }
        }

        for (auto i = 0; i < currCount; i++) {
            data[i].~T();
        }
        free(data);
        data = relocData;
    }
    else {
        if (std::is_move_assignable<T>::value == true && std::is_move_constructible<T>::value == true) {
            for (auto i = currCount - 1; i > index; i--) {
                new (&data[i + 1]) T(std::move(data[i]));
            }
        }
        else {
            for (auto i = currCount - 1; i > index; i++) {
                new (&data[i + 1]) T(data[i]);
            }
        }
        new (&relocData[index]) T(value);
    }
    currCount++;
    return index;
}

template<typename T>
void Array<T>::remove(int index) {
    if (std::is_move_assignable<T>::value == true && std::is_move_constructible<T>::value == true) {
        for (auto i = index; i < currCount - 1; i++) {
            data[i] = std::move(data[i + 1]);
        }
    }
    else {
        for (auto i = index; i < currCount - 1; i++) {
            data[i] = data[i + 1];
        }
    }
    data[index].~T();
    data[index] = nullptr;
    currCount--;
}

template<typename T>
Array<T>::Iterator Array<T>::iterator() {
    return Iterator(data, false);
}

template<typename T>
Array<T>::ConstIterator Array<T>::iterator() const {
    return Iterator(data, false);
}

template<typename T>
Array<T>::Iterator Array<T>::reverseIterator() {
    return Iterator(data, true);
}

template<typename T>
Array<T>::ConstIterator Array<T>::reverseIterator() const {
    return Iterator(data, true);
}

template<typename T>
T &Array<T>::operator[](int index) {
    return data[index];
}

template<typename T>
const T &Array<T>::operator[](int index) const {
    return data[index];
}

template<typename T>
const T& Array<T>::Iterator::get() const {
    return m_dataPtr;
}

template<typename T>
void Array<T>::Iterator::set(const T& value) {
    *m_dataPtr = value;
}

template<typename T>
void Array<T>::Iterator::next() {
    if (m_isReverse)
        m_dataPtr--;
    else
        m_dataPtr++;
}

template<typename T>
bool Array<T>::Iterator::hasNext() const {
    if (m_isReverse && m_dataPtr + 1 != nullptr)
        return true;
    else if (!m_isReverse && m_dataPtr + 1 != nullptr)
        return true;
    else
        return false;
}

template<typename T>
Array<T>::Iterator::Iterator(T *ptr, bool isReverse) {
    m_isReverse = isReverse;
    m_dataPtr = ptr;
}

template<typename T>
Array<T>::Iterator::Iterator(T *ptr) {
    m_isReverse = false;
    m_dataPtr = ptr;
}

template<typename T>
Array<T>::Array(const Array<T>& other)
  : data(nullptr), typeSize(0), capacity(0)
{
    if (other.typeSize == 0 || other.capacity == 0)
        return;
    data = (T *)malloc(other.capacity * other.typeSize);
    int i = 0;
    try {
        for (i = 0; i < other.currCount; ++i)
            data[i] = other.data[i];
        typeSize = other.typeSize;
        capacity = other.capacity;
    } catch (...) {
        for (int j = 0; j < i; ++j)
            data[j].~T();
        free(data);
        throw;
    }
}

template<typename T>
Array<T>& Array<T>::operator=(const Array<T>& rhs) {
    if (this == &rhs)
        return *this;
    vector tmp(rhs);   // makes a deep copy; may throw
    swap(tmp);         // noexcept swap: now *this has copy, tmp has old contents
    return *this;      // tmp destroyed, releasing old resources
}

vector(vector&& other) noexcept
  : data_(other.data_), size_(other.size_), capacity_(other.capacity_)
{
    other.data_ = nullptr;
    other.size_ = 0;
    other.capacity_ = 0;
}

void swap(vector& other) noexcept {
    using std::swap;
    swap(data_, other.data_);
    swap(size_, other.size_);
    swap(capacity_, other.capacity_);
}
