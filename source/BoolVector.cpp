#include "BoolVector.hpp"
#include <stdexcept>

namespace rav = ravinskij;
bool rav::BoolVector::operator[](size_t index) noexcept
{
    return bits_[index];
}

bool rav::BoolVector::operator[](size_t index) const noexcept
{
    return bits_[index];
}

bool rav::BoolVector::at(size_t index)
{
    if (index > size_)
    {
        throw std::out_of_range("out of bounds");
    }
    return bits_[index];
}

bool rav::BoolVector::at(size_t index) const
{
    if (index > size_)
    {
        throw std::out_of_range("out of bounds");
    }
    return bits_[index];
}

void rav::BoolVector::push_back(bool bit)
{
    if (size_ == capacity_)
    {
        BoolVector temp(*this);
        delete[] temp.bits_;
        temp.capacity_ *= 2;
        temp.bits_ = new bool[temp.capacity_]{};
        for (size_t i = 0; i < this->size_; ++i)
        {
            temp.bits_[i] = this->bits_[i];
        }
        std::swap(bits_, temp.bits_);
        std::swap(size_, temp.size_);
        std::swap(capacity_, temp.capacity_);
    }
    bits_[size_++] = bit;
}

void rav::BoolVector::pop_back() noexcept
{
    --size_;
}

size_t rav::BoolVector::size() const noexcept
{
    return size_;
}
