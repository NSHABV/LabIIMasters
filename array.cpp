//
// Created by Nikita Shabanov on 01.12.2025.
//
#include <cstdlib>
#include "array.h"

#include <algorithm>
#include <type_traits>

template<typename T>
Array<T>::Array() : capacity(8), typeSize(sizeof(T))  {

}

template<typename T>
Array<T>::Array(int capacity) : capacity(capacity), typeSize(sizeof(T)) {
    data = (T *)malloc(capacity * typeSize);
}

template<typename T>
Array<T>::~Array() {
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
        auto relocData = (T *)malloc(data, capacity * typeSize);
        if (std::is_move_assignable<T>::value == true && std::is_move_constructible<T>::value == true) {
            for (auto i = 0; i < currCount; i++) {
                relocData[i] = std::move(data[i]);
            }
            relocData[currCount] = std::move(value);
            free(data);
            data = (T *)relocData;
        }
        else {
            for (auto i = 0; i < currCount; i++) {
                relocData[i] = data[i];
            }
            relocData[currCount] = value;
            free(data);
            data = (T *)relocData;
        }
    }
    else
        data[currCount++] = value;
    currCount++;
}

template<typename T>
int Array<T>::insert(int index, const T& value) {
    if (currCount >= capacity) {
        capacity *= 2;

    }
}
