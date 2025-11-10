#include <memory>
#include <iostream>

class Base {
public:
    virtual ~Base() { std::cout << "Деструктор Derived\n"; }
    virtual void say() { std::cout << "Говорит Base\n"; }
};

class Derived : public Base {
public:
    ~Derived() override { std::cout << "Деструктор Derived\n"; }
    void say() override { std::cout << "Говорит Derived\n"; }
};

int main() {
    setlocale(LC_ALL, "");

    std::unique_ptr<Base> p = std::make_unique<Derived>();
    p->say();

    std::unique_ptr<Base> q = std::move(p);
    if (!p) std::cout << "p is null\n";

    return 0;
}