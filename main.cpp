#include <iostream>
#include <string>

int add(int a, int b){

    int x = a + b;
    return x;
}

int multiply(int a, int b){
    int y = a * b;
    return y;
}

/*void incrementByValue(int x, int y) {
     x = x + 1;
     y = y + 2;
     std::cout << "Inside incrementByValue: "<<x<< std::endl;
     std::cout << "Inside incrementByValue: "<<y<< std::endl;
}*/

void incrementByRef(int& x, int& y) {
    x += 1;
    y += 2;
    std::cout << "Inside incrementByRef: " <<x<<std::endl;
    std::cout << "Inside incrementByRef: " <<y<<std::endl;
    std::swap(x,y);
    std::cout << "New x is y: " <<x<<std::endl;
    std::cout << "New y is x: " <<y<<std::endl;
}

int main() {
    /*std::cout << "Hello, C++" << std::endl;
    int age;
    double height;
    char grade;
    bool isStudent;

    std::string name;
    std::cin >> name;
    std::cout << "Hello "<< name << std::endl;

    std::cin >> height;
    std::cout << "Your Height is " << height << std::endl;

    std::cin >> grade;
    std::cout << "Your grade is " << grade << std::endl;

    std::cin >>isStudent;
    std::string tmp;
    if (isStudent == 1) {
        tmp = "true";
    }
    else {
        tmp = "false";
    }

    std::cout << "Are you a student " << tmp << std::endl;
    std::cout << sizeof(name) << std::endl;
    std::cout << sizeof(height) << std::endl;
    std::cout << sizeof(grade) << std::endl;
    std::cout << sizeof(isStudent) << std::endl;*/

    int a1= add(4,5);
    int a2= multiply(1,10);
    std::cout << a1 << std::endl;
    std::cout << a2 << std::endl;
    incrementByRef(a1, a2);
    std::cout << a1 << std::endl;
    std::cout << a2 << std::endl;

    int arr[5] = {2, 4, 8, 12, 16};
    int sum = 0;

    int tmp =  arr[0];
    // printing array elements
    for (int i : arr) {
        if (i > tmp) {
            tmp = i;
        }
        sum += i;
        float avg = sum / std::size(arr);
        std::cout << sum << std::endl;
        std::cout << tmp << std::endl;
        std::cout << avg << std::endl;
    }

    std::string input;
    std::cin >> input;
    std::string reversed = "";
    for (int i = input.length() - 1; i>=0; i--){
        reversed += input[i];
        if (reversed == input){
            std::cout << "This word is a palindrome"<<std::endl;
        }
    }
    std::cout << "Reversed: "<<reversed<< std::endl;
}
