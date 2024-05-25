#ifndef BOOL_VECTOR_HPP
#define BOOL_VECTOR_HPP

#include <cstddef>

namespace ravinskij
{
    class BoolArray
    {
    public:
        BoolArray():
            vector_(0),
            currIndex_(0)
        {}
        
        BoolArray(const BoolArray& rhs) = default;
        BoolArray(BoolArray&& rhs) =  default;
        BoolArray& operator=(const BoolArray& rhs) = default;
        BoolArray& operator=(BoolArray&& rhs) = default;
        ~BoolArray() = default;

        bool operator[](size_t index);
        bool operator[](size_t index) const;
        bool at(size_t index);
        bool at(size_t index) const;
        void push_back(bool bit);
        void pop_back();
        size_t size() const;



    private:
        size_t vector_;
        size_t currIndex_;
        const size_t BOUND_ = sizeof(size_t) * 8;
    };
}

#endif