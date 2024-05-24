#ifndef BOOL_VECTOR_HPP
#define BOOL_VECTOR_HPP

#include <cstddef>
namespace ravinskij
{

    template < class T >
    class BoolVector
    {
    public:

    private:
        uint64_t *values;
        size_t size_;
        size_t top_;
    };
    
}

#endif