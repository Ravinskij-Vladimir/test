#ifndef MAP_HPP
#define MAP_HPP
#include <algorithm>
#include <initializer_list>
#include <memory>
#include <stdexcept>
#include "treeIterators.hpp"

namespace ravinskij
{
  template< typename Key, typename T, typename Compare = std::less< Key > >
  class Tree
  {
    using val_t = std::pair< Key, T >;
  public:
    using iterator = TreeIterator< Key, T, Compare >;
    using constIterator = ConstTreeIterator< Key, T, Compare >;
    Tree():
      comparator_(),
      root_(new TreeNode< Key, T >(-1)),
      size_(0)
    {}

    Tree(const Tree& other):
      comparator_(other.comparator_),
      root_(new TreeNode< Key, T >(-1)),
      size_(other.size_)
    {
      size_t added = 0;
      const TreeNode< Key, T >* cur_other = other.root_;
      TreeNode< Key, T >* cur = root_;

      try
      {
        while (size_ != added)
        {
          if (cur_other->right_)
          {
            cur->right_ = new TreeNode< Key, T >(*(cur_other->right_));
            cur->right_->parent_ = cur;
            cur = cur->right_;
            cur_other = cur_other->right_;
            ++added;
            continue;
          }
          if (cur_other->left_)
          {
            cur->left_ = new TreeNode< Key, T >(*(cur_other->left_));
            cur->left_->parent_ = cur;
            cur = cur->left_;
            cur_other = cur_other->left_;
            ++added;
            continue;
          }
          cur = cur->parent_;
          cur_other = cur_other->parent_;
        }
      }
      catch (...)
      {
        freeTreeNodes(root_);
        throw;
      }
    }

    Tree(Tree&& other) noexcept:
      comparator_(std::move(other.comparator_)),
      root_(other.root_),
      size_(other.size_)
    {
      other.root_ = nullptr;
      other.size_ = 0;
    }
    Tree(std::initializer_list< std::pair< const Key, T > > init_list):
      comparator_(),
      root_(new TreeNode< Key, T >(-1)),
      size_(0)
    {
      try
      {
        createTree(root_, init_list.begin(), init_list.end(), size_);
      }
      catch (...)
      {
        freeTreeNodes(root_);
      }
    }
    ~Tree()
    {
      freeTreeNodes(root_);
    }
    Tree& operator=(const Tree& other)
    {
      Tree new_map(other);
      clear();
      this->swap(new_map);
      return *this;
    }
    Tree& operator=(Tree&& other)
    {
      if (this == std::addressof(other))
      {
        return *this;
      }
      this->swap(other);
      other.clear();
      return *this;
    }

    iterator begin() noexcept
    {
      TreeNode< Key, T >* cur = root_;
      for (; cur->left_; cur = cur->left_);
      return iterator(cur);
    }
    constIterator begin() const noexcept
    {
      return cbegin();
    }
    constIterator cbegin() const noexcept
    {
      TreeNode< Key, T >* cur = root_;
      for (; cur->left_; cur = cur->left_);
      return constIterator(cur);
    }

    constIterator cend() const noexcept
    {
      return constIterator(root_);
    }

    iterator end() noexcept
    {
      return iterator(root_);
    }

    constIterator end() const noexcept
    {
      return constIterator(root_);
    }

    bool empty() const noexcept
    {
      return !size_;
    }

    size_t size() const noexcept
    {
      return size_;
    }

    void clear() noexcept
    {
      freeTreeNodes(root_->left_);
      root_->left_ = nullptr;
      size_ = 0;
    }

    void swap(Tree& other)
    {
      std::swap(comparator_, other.comparator_);
      std::swap(root_, other.root_);
      std::swap(size_, other.size_);
    }

    iterator find(const Key& x)
    {
      TreeNode< Key, T >* cur = root_->left_;
      while (cur)
      {
        if (cur->val_.first == x)
        {
          return iterator(cur);
        }
        cur = (comparator_(cur->val_.first, x)) ? cur->right_ : cur->left_;
      }
      return end();
    }

    constIterator find(const Key& key) const
    {
      TreeNode< Key, T >* cur = root_->left_;
      while (cur)
      {
        if (cur->val_.first == key)
        {
          return constIterator(cur);
        }
        cur = (comparator_(cur->val_.first, key)) ? cur->right_ : cur->left_;
      }
      return cend();
    }

    T& at(const Key& key)
    {
      TreeNode< Key, T >* cur = findHint(root_->left_, key);
      if (cur && cur->val_.first == key)
      {
        return cur->val_.second;
      }
      throw std::out_of_range("No such element");
    }

    const T& at(const Key& key) const
    {
      const TreeNode< Key, T >* cur = findHint(root_->left_, key);
      if (cur && cur->val_.first == key)
      {
        return cur->val_.second;
      }
      throw std::out_of_range("No such element");
    }
    T& operator[](const Key& key)
    {
      TreeNode< Key, T >* cur = findHint(root_, key);
      if (cur && cur->val_.first == key)
      {
        return cur->val_.second;
      }
      TreeNode< Key, T >* added = addTreeNode(root_, cur, key, T{});
      ++size_;
      return added->val_.second;
    }

    std::pair< iterator, bool > insert(const val_t& val)
    {
      TreeNode< Key, T >* hint = findHint(root_, val.first);
      if (hint && hint->val_.first == val.first)
      {
        return std::make_pair(iterator(hint), false);
      }
      TreeNode< Key, T >* added = addTreeNode(root_, hint, val.first, val.second);
      ++size_;
      return std::make_pair(iterator(added), true);
    }

    template< class InputIt >
    void insert(InputIt first, InputIt last)
    {
      for (; first != last; ++first)
      {
        addTreeNode(root_, nullptr, first->val_.first, first->val_.second);
      }
    }

    void insert(std::initializer_list<val_t> init_list)
    {
      insert(init_list.begin(), init_list.end());
    }

    iterator erase(iterator pos)
    {
      TreeNode< Key, T >* toDelete = pos.node_;
      ++pos;
      eraseTreeNode(toDelete);
      return pos;
    }

    size_t erase(const Key& key)
    {
      iterator pos = find(key);
      if (pos == end())
      {
        return 0;
      }
      erase(pos);
      return 1;
    }

  private:
    Compare comparator_;
    TreeNode< Key, T >* root_;
    size_t size_;
    TreeNode< Key, T>* findHint(TreeNode< Key, T >* root, const Key& key)
    {
      TreeNode< Key, T >* cur = (root->height_ < 0) ? root->left_ : root;
      while (cur)
      {
        if (cur->val_.first == key)
        {
          return cur;
        }
        if (comparator_(key, cur->val_.first))
        {
          if (!cur->left_)
          {
            return cur;
          }
          cur = cur->left_;
        }
        else
        {
          if (!cur->right_)
          {
            return cur;
          }
          cur = cur->right_;
        }
      }
      return nullptr;
    }

    const TreeNode< Key, T >* findHint(TreeNode< Key, T >* root, const Key& key) const
    {
      const TreeNode< Key, T >* cur = (root->height_ < 0) ? root->left_ : root;
      while (cur)
      {
        if (cur->val_.first == key)
        {
          return cur;
        }
        if (comparator_(key, cur->val_.first))
        {
          if (!cur->left_)
          {
            return cur;
          }
          cur = cur->left_;
        }
        else
        {
          if (!cur->right_)
          {
            return cur;
          }
          cur = cur->right_;
        }
      }
      return nullptr;
    }
    void rebalanceTree(TreeNode< Key, T >* start)
    {
      using std::abs;
      using std::max;
      while (start && start->height_ != -1)
      {
        int depth_diff = TreeNode< Key, T >::depth(start->left_) - TreeNode< Key, T >::depth(start->right_);
        if (start->height_ == max(TreeNode< Key, T >::depth(start->left_), TreeNode< Key, T >::depth(start->right_)) && abs(depth_diff) < 2)
        {
          return;
        }
        TreeNode< Key, T >* parent = start->parent_;
        if (abs(depth_diff) > 1)
        {
          start->rotate();
        }
        else
        {
          start->height_ = max(TreeNode< Key, T >::depth(start->left_), TreeNode< Key, T >::depth(start->right_));
        }
        start = parent;
      }
      return;
    }
    void freeTreeNodes(TreeNode< Key, T >* root) noexcept
    {
      if (!root)
      {
        return;
      }
      TreeNode< Key, T >* cur = root;
      while (root->right_ || root->left_)
      {
        if (cur->right_)
        {
          cur = cur->right_;
          continue;
        }
        if (cur->left_)
        {
          cur = cur->left_;
          continue;
        }
        cur = cur->parent_;
        if (cur->left_ && !cur->left_->left_ && !cur->left_->right_)
        {
          delete cur->left_;
          cur->left_ = nullptr;
        }
        if (cur->right_ && !cur->right_->left_ && !cur->right_->right_)
        {
          delete cur->right_;
          cur->right_ = nullptr;
        }
      }
      delete root;
    }
    TreeNode< Key, T >* addTreeNode(TreeNode< Key, T >* root, TreeNode< Key, T >* hint, const Key& key, const T& new_val)
    {
      if (!hint)
      {
        hint = findHint(root, key);
      }
      if (!hint)
      {
        root->left_ = new TreeNode< Key, T >(0, std::forward< val_t >(std::make_pair(key, new_val)));
        root->left_->parent_ = root;
        return root->left_;
      }
      if (hint->val_.first == key)
      {
        hint->val_.second = new_val;
        return hint;
      }
      TreeNode< Key, T >* new_node = new TreeNode< Key, T >(0, std::forward< val_t >(std::make_pair(key, new_val)));
      (comparator_(hint->val_.first, key) ? hint->right_ : hint->left_) = new_node;
      new_node->parent_ = hint;
      rebalanceTree(hint);
      return new_node;
    }
    void eraseTreeNode(TreeNode< Key, T >* toDelete)
    {
      TreeNode< Key, T >* parent = toDelete->parent_;
      if (!toDelete->left_ || !toDelete->right_)
      {
        TreeNode< Key, T >* child = (toDelete->left_ ? toDelete->left_ : toDelete->right_);
        (toDelete == parent->left_ ? parent->left_ : parent->right_) = child;
        if (child)
        {
          child->parent_ = parent;
        }
        rebalanceTree(parent);
      }
      else
      {
        TreeNode< Key, T >* prev = toDelete->left_;
        for (; prev->right_; prev = prev->right_);
        TreeNode< Key, T >* prev_parent = prev->parent_;
        prev->right_ = toDelete->right_;
        if (toDelete->right_)
        {
          toDelete->right_->parent_ = prev;
        }
        ((toDelete == parent->left_) ? parent->left_ : parent->right_) = prev;
        prev->parent_ = parent;
        if (prev_parent != toDelete)
        {
          prev_parent->right_ = prev->left_;
          prev->left_->parent_ = prev_parent;
          prev->left_ = toDelete->left_;
        }
        rebalanceTree((prev_parent != toDelete) ? prev_parent : prev);
      }
      delete toDelete;
    }
    
    template< typename InputIt >
    TreeNode< Key, T >* createTree(TreeNode< Key, T >* root, InputIt begin, InputIt end, size_t& addedCount)
    {
      addedCount = 0;
      if (begin == end)
      {
        return nullptr;
      }
      bool isRootGiven = root;
      if (!root)
      {
        root_ = new TreeNode< Key, T >(-1);
      }
      root->left_ = new TreeNode< Key, T >(0, std::forward(*begin));
      root->left_->parent_ = root;
      ++begin;
      ++addedCount;
      try
      {
        while (begin != end)
        {
          addTreeNode(root, nullptr, (*begin).first, (*begin).second);
          ++begin;
          ++addedCount;
        }
      }
      catch (...)
      {
        freeTreeNodes(isRootGiven ? root->left_ : root);
        throw;
      }
      return root;
    }
  };
}
#endif