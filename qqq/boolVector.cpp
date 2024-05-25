#include "boolVector.hpp"
#include <stdexcept>

namespace rav = ravinskij;

bool rav::BoolVector::operator[](size_t index)
{
    return vector_ & 1 << (index % (sizeof(vector_) * 8));
}

bool rav::BoolVector::operator[](size_t index) const
{
    return vector_ & 1 << (index % (sizeof(vector_) * 8));
}

bool rav::BoolVector::at(size_t index)
{
    if (index > bound_)
    {
        throw std::out_of_range("BoolVector: out of bounds");
    }
    return operator[](index);
}

bool rav::BoolVector::at(size_t index) const
{
    if (index > bound_)
    {
        throw std::out_of_range("BoolVector: out of bounds");
    }
    return operator[](index);
}

size_t rav::BoolVector::size() const
{
    return bound_;
}

void rav::BoolVector::push_back(bool bit)
{
    vector_ |= 1 << bound_++;
}

void rav::BoolVector::pop_back()
{
    --bound_;
}