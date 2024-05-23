#ifndef LIST_NODE_HPP
#define LIST_NODE_HPP
#include <utility>

namespace ravinskij
{
  namespace detail
  {
    template< class T >
    struct ListNode
    {
      template< class... Args >
      ListNode(ListNode* next, ListNode* prev, Args&& ...args):
        value_(std::forward< Args >(args)...),
        next_(next),
        prev_(prev)
      {}
      ~ListNode() = default;
      T value_;
      ListNode* next_;
      ListNode* prev_;
    };
  }
}
#endif