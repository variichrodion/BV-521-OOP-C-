#include <iostream>
#include <memory>

struct B; 

struct A {
    std::shared_ptr<B> b; 
    ~A() { std::cout << "~A\n"; }
};

struct B {
    std::shared_ptr<A> a; 
    ~B() { std::cout << "~B\n"; }
};

int main()
{
    {
        auto a = std::make_shared<A>();
        auto b = std::make_shared<B>();

        a->b = b; 
        b->a = a; 
    }

    std::cout << "end scope\n";
}