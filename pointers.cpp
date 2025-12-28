//
// Created by haris on 12/23/2025.
//

#include "pointers.h"

#include<iostream>

int main() {

    std::string myheart;
    std::cin >> myheart;
    std::string* ptr = &myheart;

    std::cout << "who has my heart: " << myheart <<std::endl;
    std::cout << "where is my heart at: " << *ptr << std::endl;


}