#include <cstddef>
#include <algorithm>
#include <cassert>

namespace ravinskij
{
    class SharedPtrCounter
    {
    public:
        SharedPtrCounter() : pn(nullptr)
        {
        }
        SharedPtrCounter(const SharedPtrCounter &count) : pn(count.pn)
        {
        }

        void swap(SharedPtrCounter &lhs) noexcept
        {
            std::swap(pn, lhs.pn);
        }
        size_t useCount() const noexcept
        {
            size_t count = 0;
            if (nullptr != pn)
            {
                count = *pn;
            }
            return count;
        }

        template <class U>
        void acquire(U *p)
        {
            if (nullptr != p)
            {
                if (nullptr == pn)
                {
                    try
                    {
                        pn = new size_t(1);
                    }
                    catch (std::bad_alloc &)
                    {
                        delete p;
                        throw;
                    }
                }
                else
                {
                    ++(*pn);
                }
            }
        }
        template <class U>
        void release(U *p) noexcept
        {
            if (nullptr != pn)
            {
                --(*pn);
                if (0 == *pn)
                {
                    delete p;
                    delete pn;
                }
                pn = nullptr;
            }
        }

    public:
        size_t *pn;
    };

    class shared_ptr_base
    {
    protected:
        shared_ptr_base(void) : pn()
        {
        }

        shared_ptr_base(const shared_ptr_base &other) : pn(other.pn)
        {
        }

        SharedPtrCounter pn;
    };

    template <class T>
    class shared_ptr : public shared_ptr_base
    {
    public:
        typedef T element_type;

        shared_ptr(void) noexcept : shared_ptr_base(),
                                    px(nullptr)
        {
        }

        explicit shared_ptr(T *p) :

                                    shared_ptr_base()
        {
            acquire(p);
        }
        template <class U>
        shared_ptr(const shared_ptr<U> &ptr, T *p) : 
            shared_ptr_base(ptr)
        {
            acquire(p);
        }
        template <class U>
        shared_ptr(const shared_ptr<U> &ptr) noexcept :

            shared_ptr_base(ptr)
        {
            assert((nullptr == ptr.get()) || (0 != ptr.useCount()));
            acquire(static_cast<typename shared_ptr<T>::element_type *>(ptr.get()));
        }

        shared_ptr(const shared_ptr &ptr) noexcept : shared_ptr_base(ptr)
        {
            assert((nullptr == ptr.px) || (0 != ptr.pn.useCount()));
            acquire(ptr.px);
        }
        shared_ptr &operator=(shared_ptr ptr) noexcept
        {
            swap(ptr);
            return *this;
        }
        ~shared_ptr(void) noexcept
        {
            release();
        }
        void reset(void) noexcept
        {
            release();
        }

        void reset(T *p)
        {
            assert((nullptr == p) || (px != p));
            release();
            acquire(p);

            void swap(shared_ptr & lhs) noexcept
            {
                std::swap(px, lhs.px);
                pn.swap(lhs.pn);
            }

            operator bool() const noexcept
            {
                return (0 < pn.useCount());
            }
            bool unique(void) const noexcept
            {
                return (1 == pn.useCount());
            }
            size_t useCount(void) const noexcept
            {
                return pn.useCount();
            }

            T &operator*() const noexcept
            {
                assert(nullptr != px);
                return *px;
            }
            T *operator->() const noexcept
            {
                assert(nullptr != px);
                return px;
            }
            T *get(void) const noexcept
            {
                return px;
            }

        private:
            void acquire(T * p)
            {
                pn.acquire(p);
                px = p;
            }
            void release(void) noexcept
            {
                pn.release(px);
                px = nullptr;
            }

        private:
            T *px;
        };

        template <class T, class U>
        bool operator==(const shared_ptr<T> &l, const shared_ptr<U> &r) noexcept
        {
            return (l.get() == r.get());
        }
        template <class T, class U>
        bool operator!=(const shared_ptr<T> &l, const shared_ptr<U> &r) noexcept
        {
            return (l.get() != r.get());
        }
        template <class T, class U>
        bool operator<=(const shared_ptr<T> &l, const shared_ptr<U> &r) noexcept
        {
            return (l.get() <= r.get());
        }
        template <class T, class U>
        bool operator<(const shared_ptr<T> &l, const shared_ptr<U> &r) noexcept
        {
            return (l.get() < r.get());
        }
        template <class T, class U>
        bool operator>=(const shared_ptr<T> &l, const shared_ptr<U> &r) noexcept
        {
            return (l.get() >= r.get());
        }
        template <class T, class U>
        bool operator>(const shared_ptr<T> &l, const shared_ptr<U> &r) noexcept
        {
            return (l.get() > r.get());
        }

        template <class T, class U>
        shared_ptr<T> static_pointer_cast(const shared_ptr<U> &ptr)
        {
            return shared_ptr<T>(ptr, static_cast<typename shared_ptr<T>::element_type *>(ptr.get()));
        }

        template <class T, class U>
        shared_ptr<T> dynamic_pointer_cast(const shared_ptr<U> &ptr)
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
    };
}