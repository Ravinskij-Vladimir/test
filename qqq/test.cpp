#include <iostream>
#include "smartPointer.hpp"

struct A
{
    int* a;
    A()
    {
        a = new int[10];
    }
    ~A()
    {
        delete[] a;
    }
};

int main()
{
    std::cout << sizeof(size_t);
    return 0;
}