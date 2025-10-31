#include <iostream>
#include <memory>

class Box {
public:
    Box(int v) : value(v) { std::cout << "Box " << value << " создан\n"; }
    ~Box() { std::cout << "Box " << value << " уничтожен\n"; }
    int value;
};

int main() {
    std::unique_ptr<Box> b1 = std::make_unique<Box>(10);
    std::cout << "Значение: " << b1->value << "\n";

    // Перемещаем владение
    std::unique_ptr<Box> b2 = std::move(b1); 
    if (!b1) std::cout << "b1 теперь пуст\n";
}