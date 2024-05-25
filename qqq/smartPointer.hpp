#ifndef SMART_POINTER_HPP
#define SMART_POINTER_HPP

#include <utility>
#include <cassert>
#include <iostream>

namespace ravinskij
{
    template <class T>
    class SmartPointer
    {
    public:
        SmartPointer():
            value_(nullptr),
            counter_(nullptr)
        {}

        SmartPointer(const SmartPointer &ptr):
            value_(ptr.value_),
            counter_(ptr.counter_)
        {
            ++*counter_;
        }

        SmartPointer(SmartPointer &&ptr):
            value_(ptr.value_),
            counter_(ptr.counter_)
        {
            ptr.value_ = nullptr;
            ptr.counter_ = nullptr;
        }    

        explicit SmartPointer(T *value):
            value_(value),
            counter_(new size_t(1))
        {}

        SmartPointer &operator=(const SmartPointer &ptr)
        {
            if (this == std::addressof(ptr))
            {
                return *this;
            }
            SmartPointer temp(ptr);
            --*counter_;
            std::swap(value_, temp.value_);
            std::swap(counter_, temp.counter_);
            return *this;
        }
        
        SmartPointer &operator=(SmartPointer &&ptr)
        {
            if (this == std::addressof(ptr))
            {
                return *this;
            }
            std::swap(value_, ptr.value_);
            std::swap(counter_, ptr.counter_);
            return *this;
        }
        
        ~SmartPointer()
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

        bool operator==(const SmartPointer& rhs) const noexcept
        {
            return *value_ == *rhs.value_;
        }

        bool operator!=(const SmartPointer& rhs) const noexcept
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
    SmartPointer<T> make_pointer(Args &&...args)
    {
        return SmartPointer<T>(new T(std::forward<Args>(args)...));
    }

}

#endif