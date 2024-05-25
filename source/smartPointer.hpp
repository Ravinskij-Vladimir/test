#ifndef SMART_POINTER_HPP
#define SMART_POINTER_HPP

#include <utility>
#include <cassert>

namespace ravinskij
{
    template <class T>
    class SmartPointer
    {
    public:
        SmartPointer() = delete;
        SmartPointer(const SmartPointer &ptr) = default;
        SmartPointer(SmartPointer &&ptr) = default;
        SmartPointer &operator=(const SmartPointer &ptr) = default;
        SmartPointer &operator=(SmartPointer &&ptr) = default;
        ~SmartPointer()
        {
            delete value_;
        }

        template <class... Args>
        explicit SmartPointer(Args &&...args) : value_(std::forward<Args>(args)...),
        {
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

    private:
        T *value_;
    };

    template <class T, class... Args>
    SmartPointer<T> make_pointer(Args &&...args)
    {
        return SmartPointer<T>(std::forward<Args>(args)...);
    }

}

#endif