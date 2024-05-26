#ifndef SHARED_PTR_HPP
#define SHARED_PTR_HPP

#include <cstddef>   
#include <algorithm> 
#include <cassert>

namespace ravinskij
{
    class SharedPtrCount
    {
    public:
        SharedPtrCount() : pointersCount(nullptr)
        {
        }
        SharedPtrCount(const SharedPtrCount &count) : pointersCount(count.pointersCount)
        {
        }
        void swap(SharedPtrCount &lhs) noexcept 
        {
            std::swap(pointersCount, lhs.pointersCount);
        }
        long use_count(void) const noexcept 
        {
            long count = 0;
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
                        pointersCount = new long(1); 
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
        long *pointersCount; 
    };

    class SharedPtrBase
    {
    protected:
        SharedPtrBase() : pointersCount()
        {
        }

        SharedPtrBase(const SharedPtrBase &other) : pointersCount(other.pointersCount)
        {
        }

        SharedPtrCount pointersCount; //!< Reference counter
    };

    /**
     * @brief minimal implementation of smart pointer, a subset of the C++11 std::shared_ptr or boost::shared_ptr.
     *
     * shared_ptr is a smart pointer retaining ownership of an object through a provided pointer,
     * and sharing this ownership with a reference counter.
     * It destroys the object when the last shared pointer pointing to it is destroyed or reset.
     */
    template <class T>
    class shared_ptr : public SharedPtrBase
    {
    public:
        /// The type of the managed object, aliased as member type
        typedef T element_type;

        /// @brief Default constructor
        shared_ptr(void) noexcept : // never throws
                                   SharedPtrBase(),
                                   px(nullptr)
        {
        }
        /// @brief Constructor with the provided pointer to manage
        explicit shared_ptr(T *p) : // may throw std::bad_alloc
                                    // px(p), would be unsafe as acquire() may throw, which would call release() in destructor
                                    SharedPtrBase()
        {
            acquire(p); // may throw std::bad_alloc
        }
        /// @brief Constructor to share ownership. Warning : to be used for pointer_cast only ! (does not manage two separate <T> and <U> pointers)
        template <class U>
        shared_ptr(const shared_ptr<U> &ptr, T *p) : // px(p), would be unsafe as acquire() may throw, which would call release() in destructor
                                                     SharedPtrBase(ptr)
        {
            acquire(p); // may throw std::bad_alloc
        }
        /// @brief Copy constructor to convert from another pointer type
        template <class U>
        shared_ptr(const shared_ptr<U> &ptr) noexcept : // never throws (see comment below)
                                                       // px(ptr.px),
                                                       SharedPtrBase(ptr)
        {
            assert((nullptr == ptr.get()) || (0 != ptr.use_count()));            // must be coherent : no allocation allowed in this path
            acquire(static_cast<typename shared_ptr<T>::element_type *>(ptr.get())); // will never throw std::bad_alloc
        }
        /// @brief Copy constructor (used by the copy-and-swap idiom)
        shared_ptr(const shared_ptr &ptr) noexcept : // never throws (see comment below)
                                                    // px(ptr.px),
                                                    SharedPtrBase(ptr)
        {
            assert((nullptr == ptr.px) || (0 != ptr.pointersCount.use_count())); // must be cohérent : no allocation allowed in this path
            acquire(ptr.px);                                              // will never throw std::bad_alloc
        }
        /// @brief Assignment operator using the copy-and-swap idiom (copy constructor and swap method)
        shared_ptr &operator=(shared_ptr ptr) noexcept // never throws
        {
            swap(ptr);
            return *this;
        }
        /// @brief the destructor releases its ownership
        ~shared_ptr(void) noexcept // never throws
        {
            release();
        }
        /// @brief this reset releases its ownership
        void reset(void) noexcept // never throws
        {
            release();
        }
        /// @brief this reset release its ownership and re-acquire another one
        void reset(T *p) // may throw std::bad_alloc
        {
            assert((nullptr == p) || (px != p)); // auto-reset not allowed
            release();
            acquire(p); // may throw std::bad_alloc
        }

        /// @brief Swap method for the copy-and-swap idiom (copy constructor and swap method)
        void swap(shared_ptr &lhs) noexcept // never throws
        {
            std::swap(px, lhs.px);
            pointersCount.swap(lhs.pointersCount);
        }

        // reference counter operations :
        operator bool() const noexcept // never throws
        {
            return (0 < pointersCount.use_count());
        }
        bool unique(void) const noexcept // never throws
        {
            return (1 == pointersCount.use_count());
        }
        long use_count(void) const noexcept // never throws
        {
            return pointersCount.use_count();
        }

        // underlying pointer operations :
        T &operator*() const noexcept // never throws
        {
            assert(nullptr != px);
            return *px;
        }
        T *operator->() const noexcept // never throws
        {
            assert(nullptr != px);
            return px;
        }
        T *get(void) const noexcept // never throws
        {
            // no assert, can return nullptr
            return px;
        }

    private:
        /// @brief acquire/share the ownership of the px pointer, initializing the reference counter
        void acquire(T *p) // may throw std::bad_alloc
        {
            pointersCount.acquire(p); // may throw std::bad_alloc
            px = p;        // here it is safe to acquire the ownership of the provided raw pointer, where exception cannot be thrown any more
        }

        /// @brief release the ownership of the px pointer, destroying the object when appropriate
        void release(void) noexcept // never throws
        {
            pointersCount.release(px);
            px = nullptr;
        }

    private:
        T *px; //!< Native pointer
    };

    // comparaison operators
    template <class T, class U>
    bool operator==(const shared_ptr<T> &l, const shared_ptr<U> &r) noexcept // never throws
    {
        return (l.get() == r.get());
    }
    template <class T, class U>
    bool operator!=(const shared_ptr<T> &l, const shared_ptr<U> &r) noexcept // never throws
    {
        return (l.get() != r.get());
    }
    template <class T, class U>
    bool operator<=(const shared_ptr<T> &l, const shared_ptr<U> &r) noexcept // never throws
    {
        return (l.get() <= r.get());
    }
    template <class T, class U>
    bool operator<(const shared_ptr<T> &l, const shared_ptr<U> &r) noexcept // never throws
    {
        return (l.get() < r.get());
    }
    template <class T, class U>
    bool operator>=(const shared_ptr<T> &l, const shared_ptr<U> &r) noexcept // never throws
    {
        return (l.get() >= r.get());
    }
    template <class T, class U>
    bool operator>(const shared_ptr<T> &l, const shared_ptr<U> &r) noexcept // never throws
    {
        return (l.get() > r.get());
    }

    // static cast of shared_ptr
    template <class T, class U>
    shared_ptr<T> static_pointer_cast(const shared_ptr<U> &ptr) // never throws
    {
        return shared_ptr<T>(ptr, static_cast<typename shared_ptr<T>::element_type *>(ptr.get()));
    }

    // dynamic cast of shared_ptr
    template <class T, class U>
    shared_ptr<T> dynamic_pointer_cast(const shared_ptr<U> &ptr) // never throws
    {
        T *p = dynamic_cast<typename shared_ptr<T>::element_type *>(ptr.get());
        if (nullptr != p)
        {
            return shared_ptr<T>(ptr, p);
        }
        else
        {
            return shared_ptr<T>();
        }
    }
}

#endif