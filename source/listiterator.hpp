#ifndef LISTITERATOR_HPP
#define LISTITERATOR_HPP
#include <memory>
#include <iterator>
#include "listnode.hpp"

namespace ravinskij
{
  template< class T >
  class List;
  template< class T >
  class ListIterator: public std::iterator< std::bidirectional_iterator_tag, T >
  {
    friend class List< T >;
  public:
    ListIterator():
      node(nullptr)
    {}
    ListIterator(const ListIterator< T >&) = default;
    ListIterator< T >& operator=(const ListIterator< T >&) = default;
    ~ListIterator() = default;
    ListIterator< T >& operator++()
    {
      node = node->next_;
      return *this;
    }
    ListIterator< T >& operator--()
    {
      node = node->prev_;
      return *this;
    }
    ListIterator< T > operator++(int)
    {
      ListIterator< T > result(*this);
      ++(*this);
      return result;
    }
    ListIterator< T > operator--(int)
    {
      ListIterator< T > result(*this);
      --(*this);
      return result;
    }
    bool operator==(const ListIterator< T >& rhs) const
    {
      return node == rhs.node;
    }
    bool operator!=(const ListIterator< T >& rhs) const
    {
      return !(*this == rhs);
    }
    T* operator->()
    {
      return std::addressof(node->value_);
    }
    T& operator*()
    {
      return node->value_;
    }
    const T* operator->() const
    {
      return std::addressof(node->value_);
    }
    const T& operator*() const
    {
      return node->value_;
    }
  private:
    ListNode< T >* node;
    explicit ListIterator(ListNode< T >* nd):
      node(nd)
    {}
  };
  template< class T >
  class ConstListIterator: public std::iterator< std::bidirectional_iterator_tag, T >
  {
    friend class List< T >;
  public:
    ConstListIterator():
      node(nullptr)
    {}
    ConstListIterator(const ConstListIterator< T >&) = default;
    ConstListIterator< T >& operator=(const ConstListIterator< T >&) = default;
    explicit ConstListIterator(ListNode< T >* nd):
      node(nd)
    {}
    ~ConstListIterator() = default;
    ConstListIterator< T >& operator++()
    {
      node = node->next_;
      return *this;
    }
    ConstListIterator< T >& operator--()
    {
      node = node->prev_;
      return *this;
    }
    ConstListIterator< T > operator++(int)
    {
      ConstListIterator< T > result(*this);
      ++(*this);
      return result;
    }
    ConstListIterator< T > operator--(int)
    {
      ConstListIterator< T > result(*this);
      --(*this);
      return result;
    }
    bool operator==(const ConstListIterator< T >& rhs) const
    {
      return node == rhs.node;
    }
    bool operator!=(const ConstListIterator< T >& rhs) const
    {
      return !(*this == rhs);
    }
    const T* operator->() const
    {
      return std::addressof(node->value_);
    }
    const T& operator*() const
    {
      return node->value_;
    }
  private:
    ListNode< T >* node;
   
  };
}
#endif