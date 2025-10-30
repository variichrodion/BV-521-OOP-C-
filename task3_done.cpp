#include <vector>
#include <algorithm>
#include <iostream>

int main() {
    setlocale(LC_ALL, "");
    std::vector<int> v = { 1, 5, 10, 15, 20 };
    int A = 5, B = 15;

    int count = std::count_if(v.begin(), v.end(), [A, B](int x) {return x >= A && x <= B;});

    std::cout << "Result: " << count << "\n";

    return 0;
}