#include <iostream>
#include "smartPointer.hpp"
#include "boolArray.hpp"

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
    using namespace ravinskij;
    BoolArray arr;
    arr.push_back(0);
    arr.push_back(1);
    std::cout << std::boolalpha;
    for (size_t i = 0; i < arr.size(); ++i)
    {
        std::cout << arr[i];
    }
    return 0;
}