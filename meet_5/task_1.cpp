#include <algorithm>
#include <iomanip>
#include <vector>
#include <iostream>

void printVec2D(const std::vector<std::vector<int>>& vec){
    for (auto row = vec.begin(); row != vec.end(); row++) {
        for (auto col = (*row).begin(); col != (*row).end(); col++)
            std::cout << std::setw(4) << *col;
        std::cout << '\n';
    }
}

int main(){
    setlocale(LC_ALL, "");
    // вектор векторов - таблица умножения
    const int table_size = 10;
    std::vector<std::vector<int>> mult_table(table_size, std::vector<int>(table_size));
    // заполняем контейнеры
    for (int i = 1; i <= table_size; i++)
        for (int j = 1; j <= table_size; j++)
            mult_table[i - 1][j - 1] = i * j;
    // printVec2D(mult_table);
    // row - строка таблицы, column - столбец таблицы
    for (auto row = mult_table.begin(); row != mult_table.end(); row++) {
        for (auto col = (*row).begin(); col != (*row).end(); col++) {
            std::cout << (*col) << ' ';
        }
        std::cout << '\n';
    }
    return 0;
}
