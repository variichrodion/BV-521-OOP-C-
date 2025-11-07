#include <iostream>
#include <stdexcept>

class Foo{
public:
    Foo() { std::cout << "Foo constructed\n"; }
    ~Foo() { std::cout << "Foo destroyed\n"; }
};

void leak() {
    Foo* p = new Foo;          
    throw std::runtime_error("boom");
    delete p;           
}

int main() {
    
    try {
        leak();
    }
    catch (...) {
        std::cout << "caught\n";
    }

    return 0;
}