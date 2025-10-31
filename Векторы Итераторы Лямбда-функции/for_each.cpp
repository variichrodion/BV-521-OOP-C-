#include <vector>
#include <algorithm>
#include <iostream>

int main() {
    setlocale(LC_ALL, "");
    std::vector<int> v = { 2, 4, 6, 8, 10 };
    int sum = 0;

    std::for_each(v.begin(), v.end(), [&sum](int x) {sum += x;});

    double avg = static_cast<double>(sum) / v.size();
    std::cout << "Average: " << avg << "\n";
    return 0;
}