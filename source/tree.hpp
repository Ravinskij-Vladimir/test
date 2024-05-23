#ifndef TREE_HPP
#define TREE_HPP
#include <algorithm>
#include <memory>
#include <initializer_list>
#include <stdexcept>
#include "treeNode.hpp"

namespace ravinskij
{
  template< typename Key, typename T, typename Compare = std::less< Key > >
  class Tree
  {
    using val_t = std::pair< const Key, T >;
    //struct Node;
    Compare comparator_;
    TreeNode<Key, T>* fakeroot_;
    size_t size_;
    // struct TreeNode
    // {
    //   val_t val_;
    //   int height_;
    //   TreeNode<Key, T>* left_;
    //   Node<Key, T>* right_;
    //   Node<Key, T>* parent_;

    //   Node():
    //     val_(),
    //     height_(0),
    //     left_(nullptr),
    //     right_(nullptr),
    //     parent_(nullptr)
    //   {}
    //   template< class... Args  >
    //   explicit Node(int height, Args&&... args):
    //     val_(std::forward< Args >(args)...),
    //     height_(height),
    //     left_(nullptr),
    //     right_(nullptr),
    //     parent_(nullptr)
    //   {}
    //   Node(const Node& other):
    //     val_(other.val_),
    //     height_(0),
    //     left_(nullptr),
    //     right_(nullptr),
    //     parent_(nullptr)
    //   {}
    //   Node(Node&& other):
    //     val_(std::move(other.val_)),
    //     height_(height_),
    //     left_(other.left_),
    //     right_(other.right_),
    //     parent_(other.parent_)
    //   {
    //     other.height_ = 0;
    //     other.left_ = nullptr;
    //     other.right_ = nullptr;
    //     other.parent_ = nullptr;
    //   }
    //   static int depth(Node<Key, T>* node)
    //   {
    //     return node ? node->height_ + 1 : 0;
    //   }
    //   void clear()
    //   {
    //     height_ = 0;
    //     left_ = nullptr;
    //     right_ = nullptr;
    //     parent_ = nullptr;
    //   }
    //   bool updateHeight()
    //   {
    //     int old_height = height_;
    //     height_ = std::max(depth(left_), depth(right_));
    //     return old_height != height_;
    //   }
    //   void rotate()
    //   {
    //     Node<Key, T>* root = this;
    //     Node<Key, T>* parent = root->parent_;
    //     bool left = (root == parent->left_);
    //     if (depth(left_) < depth(right_))
    //     {
    //       root = (depth(right_->left_) <= depth(right_->right_)) ? root->rotateLeft() : root->rotateLeftRight();
    //     }
    //     else
    //     {
    //       root = (depth(left_->right_) <= depth(left_->left_)) ? root->rotateRight() : root->rotateRightLeft();
    //     }
    //     root->parent_ = parent;
    //     (left ? parent->left_ : parent->right_) = root;
    //     return;
    //   }
    // private:
    //   Node<Key, T>* rotateLeft()
    //   {
    //     Node<Key, T>* root = this;
    //     Node<Key, T>* new_root = root->right_;
    //     root->right_ = new_root->left_;
    //     if (new_root->left_)
    //     {
    //       new_root->left_->parent_ = root;
    //     }
    //     new_root->left_ = root;
    //     root->parent_ = new_root;
    //     root->updateHeight();
    //     new_root->updateHeight();
    //     return new_root;
    //   }
    //   Node<Key, T>* rotateRight()
    //   {
    //     Node<Key, T>* root = this;
    //     Node<Key, T>* new_root = root->left_;
    //     root->left_ = new_root->right_;
    //     if (new_root->right_)
    //     {
    //       new_root->right_->parent_ = root;
    //     }
    //     new_root->right_ = root;
    //     root->parent_ = new_root;
    //     root->updateHeight();
    //     new_root->updateHeight();
    //     return new_root;
    //   }
    //   Node<Key, T>* rotateLeftRight()
    //   {
    //     Node<Key, T>* root = this;
    //     Node<Key, T>* new_root = root->right_->left_;
    //     root->right_->left_ = new_root->right_;
    //     if (new_root->right_)
    //     {
    //       new_root->right_->parent_ = root->right_;
    //     }
    //     new_root->right_ = root->right_;
    //     root->right_->parent_ = new_root;
    //     root->right_ = new_root->left_;
    //     if (new_root->left_)
    //     {
    //       new_root->left_->parent_ = root;
    //     }
    //     new_root->left_ = root;
    //     root->parent_ = new_root;
    //     root->updateHeight();
    //     new_root->right_->updateHeight();
    //     new_root->updateHeight();
    //     return new_root;
    //   }
    //   Node<Key, T>* rotateRightLeft()
    //   {
    //     Node<Key, T>* root = this;
    //     Node<Key, T>* new_root = root->left_->right_;
    //     root->left_->right_ = new_root->left_;
    //     if (new_root->left_)
    //     {
    //       new_root->left_->parent_ = root->left_;
    //     }
    //     new_root->left_ = root->left_;
    //     root->left_->parent_ = new_root;
    //     root->left_ = new_root->right_;
    //     if (new_root->right_)
    //     {
    //       new_root->right_->parent_ = root;
    //     }
    //     new_root->right_ = root;
    //     root->parent_ = new_root;
    //     root->updateHeight();
    //     new_root->left_->updateHeight();
    //     new_root->updateHeight();
    //     return new_root;
    //   }
    // };
   
    TreeNode<Key, T>* findHint(TreeNode<Key, T>* root, const Key& key)
    {
      TreeNode<Key, T>* cur = (root->height_ < 0) ? root->left_ : root;
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

    const TreeNode<Key, T>* findHint(TreeNode<Key, T>* root, const Key& key) const
    {
      TreeNode<Key, T>* cur = (root->height_ < 0) ? root->left_ : root;
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

    void rebalanceTree(TreeNode<Key, T>* start)
    {
      using std::abs;
      using std::max;
      while (start && start->height_ != -1)
      {
        int depth_diff = TreeNode<Key, T>::depth(start->left_) - TreeNode<Key, T>::depth(start->right_);
        if (start->height_ == max(TreeNode<Key, T>::depth(start->left_), TreeNode<Key, T>::depth(start->right_)) && abs(depth_diff) < 2)
        {
          return;
        }
        TreeNode<Key, T>* parent = start->parent_;
        if (abs(depth_diff) > 1)
        {
          start->rotate();
        }
        else
        {
          start->height_ = max(TreeNode<Key, T>::depth(start->left_), TreeNode<Key, T>::depth(start->right_));
        }
        start = parent;
      }
      return;
    }

    void freeNodes(TreeNode<Key, T>* root) noexcept
    {
      if (!root)
      {
        return;
      }
      TreeNode<Key, T>* cur = root;
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

    TreeNode<Key, T>* addNode(TreeNode<Key, T>* root, TreeNode<Key, T>* hint, const Key& key, const T& new_val)
    {
      if (!hint)
      {
        hint = findHint(root, key);
      }
      if (!hint)
      {
        root->left_ = new TreeNode<Key, T>(0, std::forward< val_t >(std::make_pair(key, new_val)));
        root->left_->parent_ = root;
        return root->left_;
      }
      if (hint->val_.first == key)
      {
        hint->val_.second = new_val;
        return hint;
      }
      TreeNode<Key, T>* new_node = new TreeNode<Key, T>(0, std::forward< val_t >(std::make_pair(key, new_val)));
      (comparator_(hint->val_.first, key) ? hint->right_ : hint->left_) = new_node;
      new_node->parent_ = hint;
      rebalanceTree(hint);
      return new_node;
    }

    void eraseNode(TreeNode<Key, T>* for_del)
    {
      TreeNode<Key, T>* parent = for_del->parent_;
      if (!for_del->left_ || !for_del->right_)
      {
        TreeNode<Key, T>* child = (for_del->left_ ? for_del->left_ : for_del->right_);
        (for_del == parent->left_ ? parent->left_ : parent->right_) = child;
        if (child)
        {
          child->parent_ = parent;
        }
        rebalanceTree(parent);
      }
      else
      {
        TreeNode<Key, T>* prev = for_del->left_;
        for (; prev->right_; prev = prev->right_);
        TreeNode<Key, T>* prev_parent = prev->parent_;
        prev->right_ = for_del->right_;
        if (for_del->right_)
        {
          for_del->right_->parent_ = prev;
        }
        ((for_del == parent->left_) ? parent->left_ : parent->right_) = prev;
        prev->parent_ = parent;
        if (prev_parent != for_del)
        {
          prev_parent->right_ = prev->left_;
          prev->left_->parent_ = prev_parent;
          prev->left_ = for_del->left_;
        }
        rebalanceTree((prev_parent != for_del) ? prev_parent : prev);
      }
      delete for_del;
    }

    template< typename InputIt >
    TreeNode<Key, T>* createTree(TreeNode<Key, T>* fakeroot, InputIt begin, InputIt end, size_t& nmb_of_added)
    {
      nmb_of_added = 0;
      if (begin == end)
      {
        return nullptr;
      }
      bool fake_given = fakeroot;
      if (!fakeroot)
      {
        fakeroot_ = new TreeNode<Key, T>(-1);
      }
      fakeroot->left_ = new TreeNode<Key, T>(0, std::forward(*begin));
      fakeroot->left_->parent_ = fakeroot;
      ++begin;
      ++nmb_of_added;
      try
      {
        while (begin != end)
        {
          addNode(fakeroot, nullptr, (*begin).first, (*begin).second);
          ++begin;
          ++nmb_of_added;
        }
      }
      catch (...)
      {
        freeNodes(fake_given ? fakeroot->left_ : fakeroot);
        throw;
      }
      return fakeroot;
    }

    template< bool IsConst >
    class BaseIterator
    {
      template< bool U > friend class BaseIterator;
      template< typename T1, typename T2, class T3 > friend class Tree;
      using val_t = std::conditional_t< IsConst, const std::pair< const Key, T >, std::pair< const Key, T > >;
      using prt_t = std::conditional_t< IsConst, const val_t*, val_t* >;
      using ref_t = std::conditional_t< IsConst, const val_t&, val_t& >;
      using node_t = TreeNode<Key, T>*;

      node_t node_;
    public:
      using iterator_category = std::bidirectional_iterator_tag;
      using value_type = val_t;
      using difference_type = std::ptrdiff_t;
      using pointer = prt_t;
      using reference = ref_t;

      BaseIterator() noexcept:
        node_(nullptr)
      {}
      BaseIterator(const BaseIterator& other) noexcept:
        node_(other.node_)
      {}
      explicit BaseIterator(node_t node) noexcept:
        node_(node)
      {}
      template< bool cond = IsConst, std::enable_if_t< cond, bool > = true >
      BaseIterator(const BaseIterator< !cond >& other) noexcept:
        node_(other.node_)
      {}
      BaseIterator& operator=(const BaseIterator& other) noexcept
      {
        node_ = other.node_;
      }
      BaseIterator& operator++()
      {
        if (node_->right_)
        {
          node_ = node_->right_;
          for (; node_->left_; node_ = node_->left_);
        }
        else
        {
          for (; node_ == node_->parent_->right_; node_ = node_->parent_);
          node_ = node_->parent_;
        }
        return *this;
      }
      BaseIterator operator++(int)
      {
        BaseIterator copy = *this;
        ++(*this);
        return copy;
      }
      BaseIterator& operator--()
      {
        if (node_->left_)
        {
          node_ = node_->left_;
          for (; node_->right_; node_ = node_->right_);
        }
        else
        {
          for (; node_ == node_->parent_->left_; node_ = node_->parent_);
          node_ = node_->parent_;
        }
        return *this;
      }
      BaseIterator operator--(int)
      {
        BaseIterator copy = *this;
        --(*this);
        return copy;
      }
      ref_t operator*() const
      {
        return node_->val_;
      }
      prt_t operator->() const
      {
        return std::addressof(node_->val_);
      }
      bool operator!=(const BaseIterator& other) const noexcept
      {
        return node_ != other.node_;
      }
      bool operator==(const BaseIterator& other) const noexcept
      {
        return node_ == other.node_;
      }
    };
  public:
    using iterator = BaseIterator< false >;
    using const_iterator = BaseIterator< true >;
    using reverse_iterator = std::reverse_iterator< iterator >;
    using const_reverse_iterator = std::reverse_iterator< const_iterator >;
    Tree():
      comparator_(),
      fakeroot_(new TreeNode<Key, T>(-1)),
      size_(0)
    {}
    Tree(const Tree& other):
      comparator_(other.comparator_),
      fakeroot_(new TreeNode<Key, T>(-1)),
      size_(other.size_)
    {
      size_t added = 0;
      const TreeNode<Key, T>* cur_other = other.fakeroot_;
      TreeNode<Key, T>* cur = fakeroot_;
      try
      {
        while (size_ != added)
        {
          if (cur_other->right_)
          {
            cur->right_ = new TreeNode<Key, T>(*(cur_other->right_));
            cur->right_->parent_ = cur;
            cur = cur->right_;
            cur_other = cur_other->right_;
            ++added;
            continue;
          }
          if (cur_other->left_)
          {
            cur->left_ = new TreeNode<Key, T>(*(cur_other->left_));
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
        freeNodes(fakeroot_);
        throw;
      }
    }
    Tree(Tree&& other) noexcept:
      comparator_(std::move(other.comparator_)),
      fakeroot_(other.fakeroot_),
      size_(other.size_)
    {
      other.fakeroot_ = nullptr;
      other.size_ = 0;
    }
    Tree(std::initializer_list< std::pair< const Key, T > > init_list):
      comparator_(),
      fakeroot_(new TreeNode<Key, T>(-1)),
      size_(0)
    {
      try
      {
        createTree(fakeroot_, init_list.begin(), init_list.end(), size_);
      }
      catch (...)
      {
        freeNodes(fakeroot_);
      }
    }
    ~Tree()
    {
      freeNodes(fakeroot_);
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
      TreeNode<Key, T>* cur = fakeroot_;
      for (; cur->left_; cur = cur->left_);
      return iterator(cur);
    }
    const_iterator begin() const noexcept
    {
      return cbegin();
    }
    const_iterator cbegin() const noexcept
    {
      TreeNode<Key, T>* cur = fakeroot_;
      for (; cur->left_; cur = cur->left_);
      return const_iterator(cur);
    }

    iterator end() noexcept
    {
      return iterator(fakeroot_);
    }
    const_iterator end() const noexcept
    {
      return const_iterator(fakeroot_);
    }
    const_iterator cend() const noexcept
    {
      return const_iterator(fakeroot_);
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
      freeNodes(fakeroot_->left_);
      fakeroot_->left_ = nullptr;
      size_ = 0;
    }
    void swap(Tree& other)
    {
      std::swap(comparator_, other.comparator_);
      std::swap(fakeroot_, other.fakeroot_);
      std::swap(size_, other.size_);
    }

    template< typename K>
    iterator find(const K& x)
    {
      TreeNode<Key, T>* cur = fakeroot_->left_;
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

    template< class K >
    const_iterator find(const K& x) const
    {
      TreeNode<Key, T>* cur = fakeroot_.left_;
      while (cur)
      {
        if (cur->val_.first == x)
        {
          return const_iterator(cur);
        }
        cur = (comparator_(cur->val_.first, x)) ? cur->left_ : cur->right_;
      }
      return cend();
    }

    template< class K >
    size_t count(const K& x) const
    {
      return (find(x) != end());
    }
    
    const_iterator find(const Key& key) const
    {
      TreeNode<Key, T>* cur = fakeroot_->left_;
      while (cur)
      {
        if (cur->val_.first == key)
        {
          return const_iterator(cur);
        }
        cur = (comparator_(cur->val_.first, key)) ? cur->right_ : cur->left_;
      }
      return cend();
    }

    T& operator[](const Key& key)
    {
      TreeNode<Key, T>* cur = findHint(fakeroot_, key);
      if (cur && cur->val_.first == key)
      {
        return cur->val_.second;
      }
      TreeNode<Key, T>* added = addNode(fakeroot_, cur, key, T{});
      ++size_;
      return added->val_.second;
    }

    T& at(const Key& key)
    {
      TreeNode<Key, T>* cur = findHint(fakeroot_->left_, key);
      if (cur && cur->val_.first == key)
      {
        return cur->val_.second;
      }
      throw std::out_of_range("No such element");
    }
    const T& at(const Key& key) const
    {
      const TreeNode<Key, T>* cur = findHint(fakeroot_->left_, key);
      if (cur && cur->val_.first == key)
      {
        return cur->val_.second;
      }
      throw std::out_of_range("No such element");
    }
    std::pair< iterator, bool > insert(const val_t& val)
    {
      TreeNode<Key, T>* hint = findHint(fakeroot_, val.first);
      if (hint && hint->val_.first == val.first)
      {
        return std::make_pair(iterator(hint), false);
      }
      TreeNode<Key, T>* added = addNode(fakeroot_, hint, val.first, val.second);
      ++size_;
      return std::make_pair(iterator(added), true);
    }
    iterator insert(const_iterator pos, const val_t& val)
    {
      return emplace_hint(pos, val);
    }
    template< class InputIt >
    void insert(InputIt first, InputIt last)
    {
      for (; first != last; ++first)
      {
        addNode(fakeroot_, nullptr, first->val_.first, first->val_.second);
      }
    }
    void insert(std::initializer_list<val_t> init_list)
    {
      insert(init_list.begin(), init_list.end());
    }
    iterator erase(iterator pos)
    {
      TreeNode<Key, T>* for_del = pos.node_;
      ++pos;
      eraseNode(for_del);
      return pos;
    }
    const_iterator erase(const_iterator pos)
    {
      TreeNode<Key, T>* for_del = pos.node_;
      ++pos;
      eraseNode(for_del);
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
    iterator erase(iterator first, iterator last)
    {
      for (; first != last; first = erase(first));
      return first;
    }
    const_iterator erase(const_iterator first, const_iterator last)
    {
      for (; first != last; first = erase(first));
      return first;
    }
  };
}
#endif