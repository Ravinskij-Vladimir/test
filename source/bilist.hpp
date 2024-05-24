#ifndef LIST_HPP
#define LIST_HPP
#include <cstddef>
#include <limits>
#include <stdexcept>
#include "listnode.hpp"
#include "listiterator.hpp"

namespace ravinskij
{
  template< class T >
  class List
  {
  public:
    List();
    List(const T& value, size_t count);
    template< class Iterator >
    List(Iterator start, Iterator finish);
    List(std::initializer_list< T > il);
    List(const List< T >& rhs);
    List(List< T >&& rhs);
    bool operator<(const List< T >& rhs) const;
    bool operator>=(const List< T >& rhs) const;
    bool operator<=(const List< T >& rhs) const;
    bool operator>(const List< T >& rhs) const;
    bool operator==(const List< T >& rhs) const;
    bool operator!=(const List< T >& rhs) const;
    List< T >& operator=(List< T >&& rhs);
    List< T >& operator=(const List< T >& rhs);
    ~List();
    
    void remove(const T& value);
  
    void swap(List< T >& list);
    size_t size() const noexcept;
    T& back();
    T& front();
    const T& back() const;
    const T& front() const;
    ListIterator< T > begin() noexcept;
    ListIterator< T > end() noexcept;
    ConstListIterator< T > cbegin() const noexcept;
    ConstListIterator< T > cend() const noexcept;
    ConstListIterator< T > erase(ConstListIterator< T > it);
    ListIterator< T > insert(ConstListIterator< T > it, const T& value);
    template< class Iterator >
    ListIterator< T > insert(ConstListIterator< T > it, Iterator start, Iterator finish);
    ListIterator< T > insert(ConstListIterator< T > it, size_t n, const T& value);
    ListIterator< T > insert(ConstListIterator< T > it, std::initializer_list< T > il);
    bool empty() const noexcept;
    void push_front(const T& value);
    void push_back(const T& value);
    void pop_back();
    void pop_front();
    void clear() noexcept;
    void sort();
    template< class Compare >
    void sort(Compare comp);
  private:
    ListNode< T > imaginary_node_;
    ListNode< T >* head_;
    ListNode< T >* tail_;
    size_t size_;
  };

  template< class T >
  List< T >::List():
    imaginary_node_(nullptr, nullptr, T()),
    head_(nullptr),
    tail_(nullptr),
    size_(0)
  {}
  template< class T >
  List< T >::List(const T& value, size_t count):
    List()
  {
    for (size_t i = 0; i < count; ++i)
    {
      push_back(value);
    }
  }
  template< class T >
  template< class Iterator >
  List< T >::List(Iterator start, Iterator finish):
    List()
  {
    while (start != finish)
    {
      push_back(*start++);
    }
  }
  template< class T >
  List< T >::List(std::initializer_list< T > il):
    List()
  {
    auto it = il.begin();
    while (it != il.end())
    {
      push_back(*it++);
    }
  }
  template< class T >
  List< T >::List(const List< T >& rhs):
    List()
  {
    imaginary_node_ = rhs.imaginary_node_;
    ListNode< T >* node = rhs.head_;
    while (size_ != rhs.size_)
    {
      push_back(node->value_);
      node = node->next_;
    }
  }
  template< class T >
  List< T >::List(List< T >&& rhs):
    imaginary_node_(rhs.imaginary_node_),
    head_(rhs.head_),
    tail_(rhs.tail_),
    size_(rhs.size_)
  {
    rhs.head_ = nullptr;
    rhs.tail_ = nullptr;
    rhs.size_ = 0;
  }

  template< class T >
  List< T >& List< T >::operator=(List< T >&& rhs)
  {
    if (this != std::addressof(rhs))
    {
      clear();
      swap(rhs);
    }
    return *this;
  }
  template< class T >
  List< T >& List< T >::operator=(const List< T >& rhs)
  {
    if (std::addressof(rhs) != this)
    {
      List< T > temp(rhs);
      swap(temp);
    }
    return *this;
  }
  template< class T >
  List< T >::~List()
  {
    clear();
  }

  template< class T >
  bool List< T >::operator<(const List< T >& rhs) const
  {
    size_t min_size = std::min(rhs.size_, size_);
    ListNode< T >* node = head_;
    ListNode< T >* rhs_node = rhs.head_;
    for (size_t i = 0; i < min_size; ++i)
    {
      if (node->value_ != rhs_node->value_)
      {
        return node->value_ < rhs_node->value_;
      }
      node = node->next_;
      rhs_node = rhs_node->next_;
    }
    return false;
  }
  template< class T >
  bool List< T >::operator>=(const List< T >& rhs) const
  {
    return !(*this < rhs);
  }
  template< class T >
  bool List< T >::operator<=(const List< T >& rhs) const
  {
    return !(rhs < *this);
  }
  template< class T >
  bool List< T >::operator>(const List< T >& rhs) const
  {
    return (rhs < *this);
  }
  template< class T >
  bool List< T >::operator==(const List< T >& rhs) const
  {
    size_t min_size = std::min(rhs.size_, size_);
    ListNode< T >* node = head_;
    ListNode< T >* rhs_node = rhs.head_;
    for (size_t i = 0; i < min_size; ++i)
    {
      if (!(node->value_ == rhs_node->value_))
      {
        return false;
      }
      node = node->next_;
      rhs_node = rhs_node->next_;
    }
    return rhs.size_ == size_;
  }
  template< class T >
  bool List< T >::operator!=(const List< T >& rhs) const
  {
    return !(rhs == *this);
  }
  
  
  
  template< class T >
  void List< T >::swap(List< T >& list)
  {
    std::swap(list.imaginary_node_, imaginary_node_);
    std::swap(list.head_, head_);
    std::swap(list.tail_, tail_);
    std::swap(list.size_, size_);
  }
  template< class T >
  size_t List< T >::size() const noexcept
  {
    return size_;
  }
  template< class T >
  T& List< T >::back()
  {
    return tail_->value_;
  }
  template< class T >
  T& List< T >::front()
  {
    return head_->value_;
  }
  template< class T >
  const T& List< T >::back() const
  {
    return tail_->value_;
  }
  template< class T >
  const T& List< T >::front() const
  {
    return head_->value_;
  }
  template< class T >
  ListIterator< T > List< T >::begin() noexcept
  {
    return ListIterator< T >(head_);
  }
  template< class T >
  ListIterator< T > List< T >::end() noexcept
  {
    return ListIterator< T >(std::addressof(imaginary_node_));
  }
  template< class T >
  ConstListIterator< T > List< T >::cbegin() const noexcept
  {
    return ConstListIterator< T >(head_);
  }
  template< class T >
  ConstListIterator< T > List< T >::cend() const noexcept
  {
    return ConstListIterator< T >(const_cast< ListNode< T >* >(std::addressof(imaginary_node_)));
  }
  template< class T >
  ConstListIterator< T > List< T >::erase(ConstListIterator< T > it)
  {
    ConstListIterator< T > result(it.node->next_);
    it.node->next_->prev_ = it.node->prev_;
    if (it.node == head_)
    {
      head_ = head_->next_;
    }
    else if (it.node == tail_)
    {
      tail_ = tail_->prev_;
      it.node->prev_->next_ = it.node->next_;
    }
    else
    {
      it.node->prev_->next_ = it.node->next_;
    }
    delete it.node;
    --size_;
    return result;
  }
  template< class T >
  ListIterator< T > List< T >::insert(ConstListIterator< T > it, const T& value)
  {
    if (size_ == 0)
    {
      ListNode< T >* node = new ListNode< T >{std::addressof(imaginary_node_), nullptr, value};
      imaginary_node_.prev_ = node;
      head_ = node;
      tail_ = node;
      ++size_;
      ListIterator< T > result(head_);
      return result;
    }
    ListNode< T >* node = new ListNode< T >{it.node, it.node->prev_, value};
    it.node->prev_ = node;
    if (it.node == head_)
    {
      head_ = node;
    }
    else if (it.node == tail_->next_)
    {
      tail_ = node;
      node->prev_->next_ = node;
    }
    else
    {
      node->prev_->next_ = node;
    }
    ++size_;
    ListIterator< T > result(node);
    return result;
  }
  template< class T >
  template< class Iterator >
  ListIterator< T > List< T >::insert(ConstListIterator< T > it, Iterator start, Iterator finish)
  {
    ConstListIterator< T > iterator(it);
    size_t count = 0;
    try
    {
      ListIterator< T > result(it.node);
      while (start != finish)
      {
        insert(it++, *start++);
      }
      return ++result;
    }
    catch (const std::exception& e)
    {
      --iterator;
      for (size_t i = 0; i < count; ++i)
      {
        erase(iterator--);
      }
      throw;
    }
  }
  template< class T >
  ListIterator< T > List< T >::insert(ConstListIterator< T > it, size_t n, const T& value)
  {
    ConstListIterator< T > iterator(it);
    size_t count = 0;
    try
    {
      ListIterator< T > result(it.node);
      for (size_t i = 0; i < n; ++i)
      {
        insert(it++, value);
        ++count;
      }
      return ++result;
    }
    catch (const std::exception& e)
    {
      --iterator;
      for (size_t i = 0; i < count; ++i)
      {
        erase(iterator--);
      }
      throw;
    }
  }
  template< class T >
  ListIterator< T > List< T >::insert(ConstListIterator< T > it, std::initializer_list< T > il)
  {
    return insert(it, il.begin(), il.end());
  }
  template< class T >
  bool List< T >::empty() const noexcept
  {
    return size_ == 0;
  }
  template< class T >
  void List< T >::push_front(const T& value)
  {
    insert(cbegin(), value);
  }
  template< class T >
  void List< T >::push_back(const T& value)
  {
    if (!tail_)
    {
      push_front(value);
    }
    else
    {
      insert(cend(), value);
    }
  }
  template< class T >
  void List< T >::pop_back()
  {
    erase(--cend());
  }
  template< class T >
  void List< T >::pop_front()
  {
    erase(cbegin());
  }
  template< class T >
  void List< T >::clear() noexcept
  {
    while (!empty())
    {
      pop_front();
    }
  }
  
  template< class T >
  void List< T >::sort()
  {
    auto comp = [](const T& lhs, const T& rhs)
    {
      return lhs > rhs;
    };
    sort(comp);
  }
  template< class T >
  template< class Compare >
  void List< T >::sort(Compare comp)
  {
    ListNode< T >* node1 = head_;
    for (size_t i = 0; i < size_; ++i)
    {
      ListNode< T >* node2 = node1;
      for (size_t j = i; j < size_; ++j)
      {
        if (comp(node1->value_, node2->value_))
        {
          std::swap(node1->value_, node2->value_);
        }
        node2 = node2->next_;
      }
      node1 = node1->next_;
    }
  }
}

#endif