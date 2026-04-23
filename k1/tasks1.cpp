#include <iostream>

int sum(int a, int b) {
    return a + b;
}

int main() {
    std::cout << "sum: " << sum(3,2) << std::endl;
    return 0;
}