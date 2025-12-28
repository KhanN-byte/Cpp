#include <iostream>
using namespace std;

bool isPrime(int n) {
    if (n <= 1) return false;
    for (int i = 2; i * i <= n; i++)
        if (n % i == 0) return false;
    return true;
}

int main() {
    int n;
    cout << "Enter a number: ";
    cin >> n;

    cout << (n % 2 == 0 ? "Even\n" : "Odd\n");
    cout << (isPrime(n) ? "Prime\n" : "Not Prime\n");

    return 0;
}
