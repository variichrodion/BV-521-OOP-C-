#include <iostream>
#include <vector>
#include <algorithm>

int main() {
    std::vector<int> vec{ 1,2,3,4,5,6,7,8,9 };
    std::vector<int> res(vec.size());
    std::copy_if(vec.begin(), vec.end(), res.begin(),
        [](int x) -> bool { return x % 2 == 0; });
    std::for_each(res.begin(), res.end(), [](int x) { std::cout << x << ' '; });
    std::cout << "\nsize: " << res.size() << "\n";
    return 0;
} 