#include "Node.hpp"
#include <utility>
namespace rav = ravinskij;
rav::Node::Node(memberPtr &leftNode, memberPtr &rightNode) : 
  frequency(0),
  symbol(0),
  left(leftNode),
  right(rightNode)
{
  frequency = leftNode->frequency + rightNode->frequency;
}

bool rav::NodeComparator::operator()(const memberPtr &lhs, const memberPtr &rhs) const
{
  return lhs->frequency > rhs->frequency;
}