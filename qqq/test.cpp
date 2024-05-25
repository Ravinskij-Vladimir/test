#include <iostream>
#include "smartPointer.hpp"

int main()
{
    using namespace ravinskij;
    using Ptr = SmartPointer<int>;
    Ptr ptr = make_pointer<int>(10);
    std::cout << *ptr << '\n';
    Ptr ptr2(ptr);
    std::cout << ptr2.useCount() << '\n';
    Ptr ptr3(std::move(ptr));
    std::cout << ptr2.useCount() << '\n';
    std::move(ptr3);
    std::cout << ptr2.useCount() << '\n';


    return 0;
}