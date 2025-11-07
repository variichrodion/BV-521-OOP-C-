#include <memory>
#include <iostream>

class Foo {
public:
    int value;
    Foo() : value(0) {};
    Foo(int v) : value(v) { std::cout << "Foo(" << value << ") constructed\n"; }
    ~Foo() { std::cout << "Foo(" << value << ") destroyed\n"; }
};

int main() {
    std::unique_ptr<Foo> p1 = std::make_unique<Foo>(1);
    std::unique_ptr<Foo> p2 = std::move(p1);
    if (nullptr == p1) std::cout << "p1 is null after move\n"; std::cin.get();

    Foo* raw = p2.get();
    std::cout << "raw->value = " << raw->value << "\n"; std::cin.get();

    auto takeOwnership = [](std::unique_ptr<Foo> p) {
        std::cout << "took ownership of Foo with value " << p->value << "\n";
    };
    takeOwnership(std::move(p2));
    std::cout << "leaving takeOwnership()...\n"; std::cin.get();

    p1 = std::make_unique<Foo>(1);
    auto useObjectByPtr = [](Foo* p) { 
        std::cout << "using object by pointer with value " << p->value << "\n";
    };
    useObjectByPtr(p1.get());
    std::cout << "leaving useObjectByPtr()...\n"; std::cin.get();
    
    auto useObjectByRef = [](Foo& p) {
        std::cout << "using object by reference with value " << p.value << "\n";
    };
    useObjectByRef(*p1);
    std::cout << "leaving useObjectByRef()...\n"; std::cin.get();

    const size_t arr_size = 5;
    std::unique_ptr<Foo[]> p_arr = std::make_unique<Foo[]>(5);
    for (size_t i{}; i < arr_size; i++) {
        p_arr[i].value = i + 1;
        std::cout << p_arr[i].value << ' ';
    }
    std::cout << '\n';
    return 0;
}