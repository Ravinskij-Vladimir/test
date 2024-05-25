#include "boolArray.hpp"
#include <stdexcept>

namespace rav = ravinskij;

bool rav::BoolArray::operator[](size_t index)
{
    return vector_ & 1 << (index % BOUND_);
}

bool rav::BoolArray::operator[](size_t index) const
{
    return vector_ & 1 << (index % BOUND_);
}

bool rav::BoolArray::at(size_t index)
{
    if (index > currIndex_)
    {
        throw std::out_of_range("BoolArray: out of bounds");
    }
    return operator[](index);
}

bool rav::BoolArray::at(size_t index) const
{
    if (index > currIndex_)
    {
        throw std::out_of_range("BoolArray: out of bounds");
    }
    return operator[](index);
}

size_t rav::BoolArray::size() const
{
    return currIndex_;
}

void rav::BoolArray::push_back(bool bit)
{
    if (currIndex_ == BOUND_)
    {
        throw std::logic_error("array is full");
    }
    vector_ |= bit << currIndex_++;
}

void rav::BoolArray::pop_back()
{
    --currIndex_;
}