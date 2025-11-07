#include <memory>
#include <iostream>

class Foo {
public:
    int value;
    Foo(int v) : value(v) { std::cout << "Foo(" << value << ") constructed\n"; }
    ~Foo() { std::cout << "Foo(" << value << ") destroyed\n"; }
};

int main() {
    std::shared_ptr<Foo> a = std::make_shared<Foo>(1);
    std::cout << "use_count after make_shared: " << a.use_count() << "\n";

    {
        std::shared_ptr<Foo> b = a;
        std::cout << "use_count inside block: " << a.use_count() << "\n";
        b->value();
    }

    std::cout << "use_count after block: " << a.use_count() << "\n";

    return 0;
}