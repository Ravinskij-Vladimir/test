#ifndef BOOL_VECTOR
#define BOOL_VECTOR

#include <cstddef>
#include <memory>
#include <utility>
#include <stdexcept>

namespace ravinskij
{
    class BoolVector
    {
    public:
        BoolVector():
            bits_(new bool[1]{}),
            size_(0),
            capacity_(1)
        {}

        BoolVector(const BoolVector& other):
            bits_(new bool[other.capacity_]{}),
            size_(other.size_),
            capacity_(other.capacity_)
        {
            for (size_t i = 0; i < other.size_; ++i)
            {
                bits_[i] = other.bits_[i];
            }
        }

        BoolVector(BoolVector&& other):
            bits_(other.bits_),
            size_(other.size_),
            capacity_(other.capacity_)
        {
            other.bits_ = nullptr;
        }

        BoolVector& operator=(const BoolVector& other)
        {
            if (this == std::addressof(other))
            {
                return *this;
            }
            BoolVector temp(other);
            std::swap(bits_, temp.bits_);
            std::swap(size_, temp.size_);
            std::swap(capacity_, temp.capacity_);
            return *this;
        }

        BoolVector& operator=(BoolVector&& other) noexcept
        {
            if (this == std::addressof(other))
            {
                return *this;
            }
            std::swap(bits_, other.bits_);
            std::swap(size_, other.size_);
            std::swap(capacity_, other.capacity_);
            return *this;
        }

        ~BoolVector()
        {
            delete[] bits_;
        }

        bool operator[](size_t index)
        {
            return bits_[index];
        }

        bool operator[](size_t index) const
        {
            return bits_[index];
        }

        bool at(size_t index)
        {
            if (index > size_)
            {
                throw std::out_of_range("out of bounds");
            }
            return bits_[index];
        }

         bool at(size_t index) const
        {
            if (index > size_)
            {
                throw std::out_of_range("out of bounds");
            }
            return bits_[index];
        }

        void push_back(bool bit)
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

        void pop_back()
        {
            --size_;
        }

        size_t size() const 
        {
            return size_;
        }

    private:
        bool *bits_;
        size_t size_;
        size_t capacity_;
    };
}

#endif