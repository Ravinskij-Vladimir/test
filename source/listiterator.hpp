#ifndef LISTITERATOR_HPP
#define LISTITERATOR_HPP
#include <memory>
#include <iterator>
#include "listnode.hpp"

namespace ravinskij
{
  template< class D >
  class List;
  template< class D >
  class ListIterator: public std::iterator< std::bidirectional_iterator_tag, D >
  {
    friend class List< D >;
  public:
    ListIterator():
      node(nullptr)
    {}
    ListIterator(const ListIterator< D >&) = default;
    ListIterator< D >& operator=(const ListIterator< D >&) = default;
    ~ListIterator() = default;
    ListIterator< D >& operator++()
    {
      node = node->next_;
      return *this;
    }
    ListIterator< D >& operator--()
    {
      node = node->prev_;
      return *this;
    }
    ListIterator< D > operator++(int)
    {
      ListIterator< D > result(*this);
      ++(*this);
      return result;
    }
    ListIterator< D > operator--(int)
    {
      ListIterator< D > result(*this);
      --(*this);
      return result;
    }
    bool operator==(const ListIterator< D >& rhs) const
    {
      return node == rhs.node;
    }
    bool operator!=(const ListIterator< D >& rhs) const
    {
      return !(*this == rhs);
    }
    D* operator->()
    {
      return std::addressof(node->value_);
    }
    D& operator*()
    {
      return node->value_;
    }
    const D* operator->() const
    {
      return std::addressof(node->value_);
    }
    const D& operator*() const
    {
      return node->value_;
    }
  private:
    detail::ListNode< D >* node;
    explicit ListIterator(detail::ListNode< D >* nd):
      node(nd)
    {}
  };
  template< class D >
  class ConstListIterator: public std::iterator< std::bidirectional_iterator_tag, D >
  {
    friend class List< D >;
  public:
    ConstListIterator():
      node(nullptr)
    {}
    ConstListIterator(const ConstListIterator< D >&) = default;
    ConstListIterator< D >& operator=(const ConstListIterator< D >&) = default;
    ~ConstListIterator() = default;
    ConstListIterator< D >& operator++()
    {
      node = node->next_;
      return *this;
    }
    ConstListIterator< D >& operator--()
    {
      node = node->prev_;
      return *this;
    }
    ConstListIterator< D > operator++(int)
    {
      ConstListIterator< D > result(*this);
      ++(*this);
      return result;
    }
    ConstListIterator< D > operator--(int)
    {
      ConstListIterator< D > result(*this);
      --(*this);
      return result;
    }
    bool operator==(const ConstListIterator< D >& rhs) const
    {
      return node == rhs.node;
    }
    bool operator!=(const ConstListIterator< D >& rhs) const
    {
      return !(*this == rhs);
    }
    const D* operator->() const
    {
      return std::addressof(node->value_);
    }
    const D& operator*() const
    {
      return node->value_;
    }
  private:
    detail::ListNode< D >* node;
    explicit ConstListIterator(detail::ListNode< D >* nd):
      node(nd)
    {}
  };
}
#endif