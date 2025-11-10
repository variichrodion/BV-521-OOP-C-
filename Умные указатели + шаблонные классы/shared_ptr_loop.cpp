#include <memory>
#include <iostream>

class Derived;

class Base {
public:
    std::shared_ptr<Derived> derived_ptr;
    virtual ~Base() { std::cout << "Деструктор Base\n"; }
    virtual void say() { std::cout << "Говорит Base\n"; }
};

class Derived : public Base {
public:
    std::shared_ptr<Base> base_ptr;
    ~Derived() override { std::cout << "Деструктор Derived\n"; }
    void say() override { std::cout << "Говорит Derived\n"; }
};

int main() {
    setlocale(LC_ALL, "");

    auto parent = std::make_shared<Base>();
    auto child = std::make_shared<Derived>();

    child->base_ptr = parent;
    parent->derived_ptr = child;


    std::cout << "End of main\n";
    return 0;
}