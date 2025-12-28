//
// Created by haris on 12/23/2025.
//

#include "dynamic_array.h"

#include<iostream>

int main() {

    int size;
    std::cout << "Enter size of elements: ";
    std::cin >> size;

    int* arr = new int[size];

    std::cout << "Enter " << size << " Integers: " << std::endl;
    for (int i = 0; i < size; i++) {
        std::cin >> arr[i];
    }
    std::cout << "Array values: ";
    for (int i = 0; i < size; i++){
        std::cout << arr[i] << " ";
    }
    std::cout << std::endl;

    delete[] arr;
    arr = nullptr;
    return 0;

};



