#ifndef TREE_NODE_HPP
#define TREE_NODE_HPP
#include <utility>

namespace ravinskij
{
template < class Key, class T >
struct TreeNode
    {
      using val_type = std::pair< Key, T >;
      val_type val_;
      int height_;
      TreeNode* left_;
      TreeNode* right_;
      TreeNode* parent_;

      TreeNode():
        val_(),
        height_(0),
        left_(nullptr),
        right_(nullptr),
        parent_(nullptr)
      {}
      template< class... Args  >
      explicit TreeNode(int height, Args&&... args):
        val_(std::forward< Args >(args)...),
        height_(height),
        left_(nullptr),
        right_(nullptr),
        parent_(nullptr)
      {}
      TreeNode(const TreeNode& other):
        val_(other.val_),
        height_(0),
        left_(nullptr),
        right_(nullptr),
        parent_(nullptr)
      {}
      TreeNode(TreeNode&& other):
        val_(std::move(other.val_)),
        height_(height_),
        left_(other.left_),
        right_(other.right_),
        parent_(other.parent_)
      {
        other.height_ = 0;
        other.left_ = nullptr;
        other.right_ = nullptr;
        other.parent_ = nullptr;
      }
      static int depth(TreeNode* node)
      {
        return node ? node->height_ + 1 : 0;
      }
      void clear()
      {
        height_ = 0;
        left_ = nullptr;
        right_ = nullptr;
        parent_ = nullptr;
      }
      bool updateHeight()
      {
        int old_height = height_;
        height_ = std::max(depth(left_), depth(right_));
        return old_height != height_;
      }
      void rotate()
      {
        TreeNode* root = this;
        TreeNode* parent = root->parent_;
        bool left = (root == parent->left_);
        if (depth(left_) < depth(right_))
        {
          root = (depth(right_->left_) <= depth(right_->right_)) ? root->rotateLeft() : root->rotateLeftRight();
        }
        else
        {
          root = (depth(left_->right_) <= depth(left_->left_)) ? root->rotateRight() : root->rotateRightLeft();
        }
        root->parent_ = parent;
        (left ? parent->left_ : parent->right_) = root;
        return;
      }
    private:
      TreeNode* rotateLeft()
      {
        TreeNode* root = this;
        TreeNode* newRoot = root->right_;
        root->right_ = newRoot->left_;
        if (newRoot->left_)
        {
          newRoot->left_->parent_ = root;
        }
        newRoot->left_ = root;
        root->parent_ = newRoot;
        root->updateHeight();
        newRoot->updateHeight();
        return newRoot;
      }
      TreeNode* rotateRight()
      {
        TreeNode* root = this;
        TreeNode* newRoot = root->left_;
        root->left_ = newRoot->right_;
        if (newRoot->right_)
        {
          newRoot->right_->parent_ = root;
        }
        newRoot->right_ = root;
        root->parent_ = newRoot;
        root->updateHeight();
        newRoot->updateHeight();
        return newRoot;
      }
      TreeNode* rotateLeftRight()
      {
        TreeNode* root = this;
        TreeNode* newRoot = root->right_->left_;
        root->right_->left_ = newRoot->right_;
        if (newRoot->right_)
        {
          newRoot->right_->parent_ = root->right_;
        }
        newRoot->right_ = root->right_;
        root->right_->parent_ = newRoot;
        root->right_ = newRoot->left_;
        if (newRoot->left_)
        {
          newRoot->left_->parent_ = root;
        }
        newRoot->left_ = root;
        root->parent_ = newRoot;
        root->updateHeight();
        newRoot->right_->updateHeight();
        newRoot->updateHeight();
        return newRoot;
      }
      TreeNode* rotateRightLeft()
      {
        TreeNode* root = this;
        TreeNode* newRoot = root->left_->right_;
        root->left_->right_ = newRoot->left_;
        if (newRoot->left_)
        {
          newRoot->left_->parent_ = root->left_;
        }
        newRoot->left_ = root->left_;
        root->left_->parent_ = newRoot;
        root->left_ = newRoot->right_;
        if (newRoot->right_)
        {
          newRoot->right_->parent_ = root;
        }
        newRoot->right_ = root;
        root->parent_ = newRoot;
        root->updateHeight();
        newRoot->left_->updateHeight();
        newRoot->updateHeight();
        return newRoot;
      }
    };
}
#endif