#ifndef SMART_POINTER_HPP
#define SMART_POINTER_HPP

#include <utility>
#include <cassert>
#include <iostream>

namespace ravinskij
{
    template <class T>
    class SharedPointer
    {
    public:
        SharedPointer():
            value_(nullptr),
            counter_(nullptr)
        {}

        SharedPointer(const SharedPointer &ptr):
            value_(ptr.value_),
            counter_(ptr.counter_)
        {
            if (ptr.value_)
            {
                ++*counter_;
            }
        }

        SharedPointer(SharedPointer &&ptr):
            value_(ptr.value_),
            counter_(ptr.counter_)
        {
            ptr.value_ = nullptr;
            ptr.counter_ = nullptr;
        }    

        explicit SharedPointer(T *value):
            value_(value),
            counter_(new size_t(1))
        {}

        SharedPointer &operator=(const SharedPointer &ptr)
        {
            if (this == std::addressof(ptr))
            {
                return *this;
            }
            SharedPointer temp(ptr);
            if (ptr.value_)
            {
                --*counter_;
            }
            std::swap(value_, temp.value_);
            std::swap(counter_, temp.counter_);
            return *this;
        }
        
        SharedPointer &operator=(SharedPointer &&ptr)
        {
            if (this == std::addressof(ptr))
            {
                return *this;
            }
            std::swap(value_, ptr.value_);
            std::swap(counter_, ptr.counter_);
            return *this;
        }
        
        ~SharedPointer()
        {
            if (value_ != nullptr)
            {
                --*counter_;
                
                if (*counter_ == 0)
                {
                    delete value_;
                    delete counter_;
                }
            }
            
        }

        const T &operator*() const noexcept
        {
            assert(nullptr != value_);
            return *value_;
        }
        const T *operator->() const noexcept
        {
            assert(nullptr != value_);
            return value_;
        }

        T &operator*() noexcept
        {
            assert(nullptr != value_);
            return *value_;
        }
        T *operator->() noexcept
        {
            assert(nullptr != value_);
            return value_;
        }

        explicit operator bool() const noexcept
        {
            return value_ != nullptr;
        }

        bool operator==(const SharedPointer& rhs) const noexcept
        {
            return value_ == rhs.value_;
        }

        bool operator!=(const SharedPointer& rhs) const noexcept
        {
            return !(*this == rhs);
        }

        size_t useCount() const noexcept
        {
            return *counter_;
        }

    private:
        T *value_;
        size_t *counter_;
    };

    template <class T, class... Args>
    SharedPointer<T> make_shared(Args &&...args)
    {
        return SharedPointer<T>(new T(std::forward<Args>(args)...));
    }

}

#endif