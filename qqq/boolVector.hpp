#ifndef BOOL_VECTOR_HPP
#define BOOL_VECTOR_HPP

#include <cstddef>

namespace ravinskij
{
    class BoolVector
    {
    public:
        BoolVector() = default;
        BoolVector(const BoolVector& rhs) = default;
        BoolVector(BoolVector&& rhs) =  default;
        BoolVector& operator=(const BoolVector& rhs) = default;
        BoolVector& operator=(BoolVector&& rhs) = default;
        ~BoolVector() = default;

        bool operator[](size_t index);
        bool operator[](size_t index) const;
        bool at(size_t index);
        bool at(size_t index) const;
        void push_back(bool bit);
        void pop_back();
        size_t size() const;



    private:
        size_t vector_;
        size_t bound_;

    };
}

#endif