#ifndef NODE_HPP
#define NODE_HPP
#include <cstddef>
#include "smartPointer.hpp"

namespace ravinskij
{
  struct Node;

  using nodePtr = SmartPointer<Node>;
  using memberPtr = SmartPointer<Node>;
  struct Node
  {
    size_t frequency;
    char symbol;
    memberPtr left, right;

    Node() = default;
    Node(const Node &) = default;
    Node(Node &&) = default;
    Node &operator=(const Node &) = default;
    Node &operator=(Node &&) = default;
    ~Node() = default;

    Node(memberPtr& leftNode, memberPtr& rightNode);
  private:  
  };
  
  struct NodeComparator
  {
    bool operator()(const memberPtr& lhs, const memberPtr& rhs) const;
  };
}

#endif