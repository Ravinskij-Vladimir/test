#include <iostream>
#include <chrono>
#include "tree.hpp"
#include "bilist.hpp"


void benchTree()
{
    using namespace ravinskij;
    Tree< int, int > ints;
    const size_t CYCLES = 100;
    const size_t STEP = 1000;
    std::cout << "Cleared elements  Time difference [µs]\n";
    for (size_t i = 0; i < CYCLES; ++i)
    {
        //std::chrono::steady_clock::time_point beginIns = std::chrono::steady_clock::now();
        for (size_t j = 0; j < STEP * i; ++j)
        {
            ints.insert({i, j});
        }
        //std::chrono::steady_clock::time_point endIns = std::chrono::steady_clock::now();
        std::chrono::steady_clock::time_point beginClear = std::chrono::steady_clock::now();
        ints.clear();
        std::chrono::steady_clock::time_point endClear = std::chrono::steady_clock::now();
        
        std::cout <<  STEP * i << "\t\t\t";
        std::cout << std::chrono::duration_cast<std::chrono::microseconds>(endClear - beginClear).count() << '\n';
    }
}

void benchList()
{
    using namespace ravinskij;
    List< size_t > ints;
    const size_t CYCLES = 100;
    const size_t STEP = 1000;
    std::cout << "Cleared elements  Time difference [µs]\n";
    for (size_t i = 0; i < CYCLES; ++i)
    {
        std::chrono::steady_clock::time_point beginIns = std::chrono::steady_clock::now();
        for (size_t j = 0; j < STEP * i; ++j)
        {
            ints.push_back(i);
        }
        std::chrono::steady_clock::time_point endIns = std::chrono::steady_clock::now();
        //std::chrono::steady_clock::time_point beginClear = std::chrono::steady_clock::now();
        ints.clear();
        //std::chrono::steady_clock::time_point endClear = std::chrono::steady_clock::now();
        
        std::cout <<  STEP * i << "\t\t\t";
        std::cout << std::chrono::duration_cast<std::chrono::microseconds>(endIns - beginIns).count() << '\n';
    }
}
// int main()
// {
//     benchList();
// }