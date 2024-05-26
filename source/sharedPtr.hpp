#ifndef SHARED_PTR_HPP
#define SHARED_PTR_HPP

#include <cstddef>
#include <memory>
#include <algorithm> 
#include <cassert>

namespace ravinskij
{
    class SharedPtrCount
    {
    public:
        SharedPtrCount() : pointersCount(nullptr)
        {}

        SharedPtrCount(const SharedPtrCount &count): 
            pointersCount(count.pointersCount)
        {}

        SharedPtrCount(SharedPtrCount &&count) noexcept:
            pointersCount(count.pointersCount)
        {
            count.pointersCount = nullptr;
        }

        SharedPtrCount& operator=(const SharedPtrCount &count)
        {
            if (this == std::addressof(count))
            {
                return *this;
            }
            SharedPtrCount temp(count);
            swap(temp);
            return *this;
        }

        SharedPtrCount& operator=(SharedPtrCount &&count) noexcept
        {
            if (this == std::addressof(count))
            {
                return *this;
            }
            swap(count);
            return *this;
        }

        void swap(SharedPtrCount &lhs) noexcept 
        {
            std::swap(pointersCount, lhs.pointersCount);
        }

        size_t useCount() const noexcept 
        {
            size_t count = 0;
            if (nullptr != pointersCount)
            {
                count = *pointersCount;
            }
            return count;
        }

        template <class U>
        void acquire(U *p) 
        {
            if (nullptr != p)
            {
                if (nullptr == pointersCount)
                {
                    try
                    {
                        pointersCount = new size_t(1); 
                    }
                    catch (std::bad_alloc &)
                    {
                        delete p;
                        throw; 
                    }
                }
                else
                {
                    ++(*pointersCount);
                }
            }
        }
        template <class U>
        void release(U *p) noexcept 
        {
            if (nullptr != pointersCount)
            {
                --(*pointersCount);
                if (0 == *pointersCount)
                {
                    delete p;
                    delete pointersCount;
                }
                pointersCount = nullptr;
            }
        }

    public:
        size_t *pointersCount; 
    };

    class SharedPtrBase
    {
    protected:
        SharedPtrBase() : pointersCount()
        {}

        SharedPtrBase(const SharedPtrBase &other) : 
            pointersCount(other.pointersCount)
        {}

        SharedPtrBase(SharedPtrBase &&other) noexcept:
            pointersCount(other.pointersCount)
        {
            other.pointersCount.pointersCount = nullptr;
        }

        SharedPtrBase& operator=(const SharedPtrBase &other)
        {
            if (this == std::addressof(other))
            {
                return *this;
            }
            SharedPtrBase temp(other);
            std::swap(this->pointersCount, temp.pointersCount);
            return *this;
        }

        SharedPtrBase& operator=(SharedPtrBase &&other) noexcept
        {
            if (this == std::addressof(other))
            {
                return *this;
            }
            std::swap(this->pointersCount, other.pointersCount);
            return *this;
        }

        SharedPtrCount pointersCount;
    };

    template <class T>
    class SharedPtr : public SharedPtrBase
    {
    public:

        SharedPtr() noexcept : 
            SharedPtrBase(),
            pointer(nullptr)
        {}
        explicit SharedPtr(T *p) :
            SharedPtrBase()
        {
            acquire(p);
        }

        template <class U>
        SharedPtr(const SharedPtr<U> &ptr): 
            SharedPtrBase(ptr)
        {
            // if(!((nullptr == p) || (pointer != p)))
            // {
            //     throw std::invalid_argument("void reset(T *p): invalid pointer to use")
            // } 
            assert((ptr.get == nullptr) || (ptr.useCount != 0));            
            acquire(static_cast<T *>(ptr.get())); 
        }

        SharedPtr(const SharedPtr &ptr): 
            SharedPtrBase(ptr)
        {
            assert((ptr.pointer == nullptr) || (ptr.pointersCount.useCount() != 0)); 
            acquire(ptr.pointer);                                              
        }

        SharedPtr(SharedPtr &&ptr) noexcept: 
            SharedPtrBase(ptr),
            pointer(ptr.pointer)
        {
            ptr.pointer = nullptr;
            ptr.pointersCount.pointersCount = nullptr;
        }

        SharedPtr &operator=(const SharedPtr& ptr)
        {
            if (this == std::addressof(ptr))
            {
                return *this;
            }
            SharedPtr temp(ptr);
            swap(temp);
            return *this;
        }

        SharedPtr &operator=(SharedPtr&& ptr) noexcept
        {
             if (this == std::addressof(ptr))
            {
                return *this;
            }
            swap(ptr);
            return *this;
        }

        ~SharedPtr() noexcept 
        {
            release();
        }

        void reset() noexcept 
        {
            release();
        }

        void reset(T *p) 
        {
            assert((p == nullptr) || (pointer != p)); 
            release();
            acquire(p); 
        }

        void swap(SharedPtr &lhs) noexcept 
        {
            std::swap(pointer, lhs.pointer);
            pointersCount.swap(lhs.pointersCount);
        }

        operator bool() const noexcept 
        {
            return (0 < pointersCount.useCount());
        }

        bool unique() const noexcept 
        {
            return (1 == pointersCount.useCount());
        }

        size_t useCount() const noexcept 
        {
            return pointersCount.useCount();
        }

        T &operator*() const noexcept 
        {
            assert(pointer != nullptr);
            return *pointer;
        }

        T *operator->() const noexcept 
        {
            assert(pointer != nullptr);
            return pointer;
        }

        T *get() const noexcept 
        {
            return pointer;
        }

    private:
        void acquire(T *p) 
        {
            pointersCount.acquire(p); 
            pointer = p;        
        }

        void release() noexcept 
        {
            pointersCount.release(pointer);
            pointer = nullptr;
        }

    private:
        T *pointer; 
    };

    template <class T, class U>
    bool operator==(const SharedPtr<T> &l, const SharedPtr<U> &r) noexcept 
    {
        return (l.get() == r.get());
    }
    template <class T, class U>
    bool operator!=(const SharedPtr<T> &l, const SharedPtr<U> &r) noexcept
    {
        return (l.get() != r.get());
    }

    template <class T, class U>
    bool operator>(const SharedPtr<T> &l, const SharedPtr<U> &r) noexcept 
    {
        return (l.get() > r.get());
    }
}

#endif