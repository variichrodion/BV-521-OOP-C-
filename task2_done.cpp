#include <vector>
#include <string>
#include <algorithm>
#include <iostream>

int main() {
    setlocale(LC_ALL, "");
    std::vector<std::string> v =
        { "apple", "kiwi", "banana", "fig", "watermelon", "orange", "cherry", "appricot" };
    // По возрастанию длины
    std::sort(v.begin(), v.end(), [](const std::string& a, const std::string& b) {
        return a.size() < b.size();
    });

    // Вывод
    for (auto& s : v) std::cout << s << " ";
    std::cout << "\n";

    // По убыванию длины
    std::sort(v.begin(), v.end(), [](const std::string& a, const std::string& b) {
        return a.size() > b.size();
    });

    // Вывод
    for (auto& s : v) std::cout << s << " ";
    std::cout << "\n";
    return 0;
}
