#include <iostream>
#include <vector>
#include <algorithm>

int main() {
    std::vector<int> vec{ 9,-1,10,-3,-2,7,-8 };
    std::for_each(vec.begin(), vec.end(), [](int x) { std::cout << x << ' '; });
    std::cout << '\n';
    auto it = std::remove_if(vec.begin(), vec.end(), [](int x) { return x < 0; });
    std::for_each(vec.begin(), vec.end(), [](int x) { std::cout << x << ' '; });
    std::cout << '\n';
    vec.erase(it, vec.end());
    std::for_each(vec.begin(), vec.end(), [](int x) { std::cout << x << ' '; });
    std::cout << '\n';
    return 0;
} 