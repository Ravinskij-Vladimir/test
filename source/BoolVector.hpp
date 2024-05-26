#ifndef BOOL_VECTOR
#define BOOL_VECTOR

#include <cstddef>
#include <memory>
#include <utility>

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

        bool operator[](size_t index) noexcept;

        bool operator[](size_t index) const noexcept;

        bool at(size_t index);

         bool at(size_t index) const;

        void push_back(bool bit);

        void pop_back() noexcept;

        size_t size() const noexcept;

    private:
        bool *bits_;
        size_t size_;
        size_t capacity_;
    };
}

#endif