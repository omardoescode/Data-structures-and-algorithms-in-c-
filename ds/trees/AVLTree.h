#pragma once
#include <iostream>
#include <stdexcept>
#include <utility>

// A node's balance is defined as the difference between the height of the left
// and right subtree
// A tree is balanced if the height of each node is in -1, 0, or 1
// Constraints on data type `T`
// 1. Must support the < operator
// 2. Must support the == operator
// 3. Must support the << operator
template <typename T> class AVLTree {
private:
  static constexpr int ALLOWED_IMBALANCE = 1;
  struct AVLNode {
    T elem;
    AVLNode *left;
    AVLNode *right;
    int height;

    AVLNode(const T &elem, AVLNode *l = nullptr, AVLNode *r = nullptr,
            int height = 0)
        : elem{elem}, left{l}, right{r}, height{height} {}
    AVLNode(T &&elem, AVLNode *l = nullptr, AVLNode *r = nullptr,
            int height = 0)
        : elem{std::move(elem)}, left{l}, right{r}, height{height} {}
  };

public:
  // Constructor
  AVLTree() : _root{nullptr} {}

  // Copy Constructor
  AVLTree(const AVLTree<T> &rhs) { _root = clone(rhs._root); }

  // Move Constructor
  AVLTree(AVLTree<T> &&rhs) {
    _root = rhs._root;
    rhs._root = nullptr;
  }

  // Copy Assignment
  AVLTree<T> &operator=(const AVLTree<T> &rhs) {
    clear(_root);
    _root = clone(rhs._root);
  }

  // Move Assignment
  AVLTree<T> &operator=(AVLTree<T> &rhs) {
    clear(_root);
    _root = rhs._root;
    rhs._root = nullptr;
  }

  // returns the minimum element in an AVL tree
  const T &findMin() const { return findMin(_root); }
  // returns the maximum element in an AVL tree
  const T &findMax() const { return findMax(_root); }

  // return true if `x` is found in the tree
  bool contains(const T &x) const { return contains(_root, x); }
  bool contains(T &&x) const { return contains(_root, std::move(x)); }

  // return tree if tree is empty
  bool empty() const { return _root == nullptr; }

  // remove every element in the tree
  void clear() { clear(_root); }

  // inserts `value` element in the tree
  // returns true if the element is successfully inserted, false if it's
  // aduplicate
  bool insert(const T &value) { return insert(_root, value); }
  bool insert(T &&value) { return insert(_root, std::move(value)); }

  // removes `value` from the tree
  // returns true if the element is successfully found and removed, false if
  // it's not there
  bool remove(const T &value) { return remove(_root, value); }
  bool remove(T &&value) { return remove(_root, std::move(value)); }

  // Operator overloading for the insertion opertor to print tree
  // A wrapper around AvlTree<T>::printTree
  // No Idea why we need another template, but hey it works (happy face).
  template <typename U>
  friend std::ostream &operator<<(std::ostream &, const AVLTree<U> &);

private:
  const T &findMin(AVLNode *tree) const {
    if (!tree)
      throw std::runtime_error("nullptr exception");

    if (!tree->left)
      return tree->elem;
    return findMin(tree->left);
  }

  const T &findMax(AVLNode *tree) const {
    if (!tree)
      throw std::runtime_error("nullptr exception");

    if (!tree->right)
      return tree->elem;
    return findMax(tree->right);
  }

  bool contains(AVLNode *tree, const T &target) const {
    if (!tree)
      return false;
    if (tree->elem > target)
      return contains(tree->left, target);
    if (tree->elem < target)
      return contains(tree->right, target);
    return true;
  }

  bool contains(AVLNode *tree, T &&target) const {
    if (!tree)
      return false;
    if (tree->elem > target)
      return contains(tree->left, std::move(target));
    if (tree->elem < target)
      return contains(tree->right, std::move(target));
    return true;
  }

  void clear(AVLNode *&tree) {
    if (!tree)
      return;
    clear(tree->left);
    clear(tree->right);
    delete tree;
    tree = nullptr;
  }

  AVLNode *clone(AVLNode *t) const {
    return t == nullptr ? nullptr
                        : new AVLNode(t->elem, clone(t->left), clone(t->right));
  }

  bool insert(AVLNode *&tree, const T &value) {
    bool inserted = false;
    if (!tree) {
      tree = new AVLNode(value);
      inserted = true;
    } else if (value < tree->elem)
      inserted = insert(tree->left, value);
    else if (value > tree->elem)
      inserted = insert(tree->right, value);
    else
      ; // duplication case, inserted=false;

    apply_rotation(tree);
    return inserted;
  }

  bool insert(AVLNode *&tree, T &&value) {
    bool inserted = false;
    if (!tree) {
      tree = new AVLNode(std::move(value));
      inserted = true;
    } else if (value < tree->elem)
      inserted = insert(tree->left, std::move(value));
    else if (value > tree->elem)
      inserted = insert(tree->right, std::move(value));
    else
      ; // duplication case, inserted=false;

    apply_rotation(tree);
    return inserted;
  }

  int height(AVLNode *node) { return !node ? -1 : node->height; }

  int calc_balance(AVLNode *tree) {
    if (!tree)
      return 0;
    return height(tree->left) - height(tree->right);
  }

  void apply_rotation(AVLNode *&t) {
    if (!t)
      return;

    int balance = calc_balance(t);

    if (balance > ALLOWED_IMBALANCE) {
      if (height(t->left->left) >= height(t->left->right))
        rotateWithLeftChild(t);
      else
        doubleRotateWithLeftChild(t);
    } else if (balance < -ALLOWED_IMBALANCE) {
      if (height(t->right->right) >= height(t->right->left))
        rotateWithRightChild(t);
      else
        doubleRotateWithRightChild(t);
    }

    t->height = 1 + std::max(height(t->left), height(t->right));
  }

  void rotateWithLeftChild(AVLNode *&k2) {
    AVLNode *k1 = k2->left;
    k2->left = k1->right;
    k1->right = k2;
    k2->height = 1 + std::max(height(k2->left), height(k2->right));
    k1->height = 1 + std::max(height(k1->left), height(k1->right));
    k2 = k1;
  }

  void rotateWithRightChild(AVLNode *&k1) {
    AVLNode *k2 = k1->right;
    k1->right = k2->left;
    k2->left = k1;
    k1->height = 1 + std::max(height(k1->left), height(k1->right));
    k2->height = 1 + std::max(height(k2->left), height(k2->right));
    k1 = k2;
  }

  void doubleRotateWithLeftChild(AVLNode *&k3) {
    rotateWithRightChild(k3->left);
    rotateWithLeftChild(k3);
  }

  void doubleRotateWithRightChild(AVLNode *&k3) {
    rotateWithLeftChild(k3->right);
    rotateWithRightChild(k3);
  }

  bool remove(AVLNode *&t, const T &value) {
    bool removed = false;
    if (t == nullptr)
      ; // do nothing
    else if (value < t->elem)
      removed = remove(t->left, value);
    else if (value > t->elem)
      removed = remove(t->right, value);
    else if (t->left != nullptr && t->right != nullptr) {
      t->elem = findMin(t->right);
      removed =
          remove(t->right, t->elem); // Must return true, since already in tree
    } else {                         // Case of one child or no children
      AVLNode *old = t;
      t = (t->left != nullptr ? t->left : t->right);
      delete old;
      removed = true;
    }

    apply_rotation(t);
    return removed;
  }

  bool remove(AVLNode *&t, T &&value) {
    bool removed = false;
    if (t == nullptr)
      ; // do nothing
    else if (value < t->elem)
      removed = remove(t->left, std::move(value));
    else if (value > t->elem)
      removed = remove(t->right, std::move(value));
    else if (t->left != nullptr && t->right != nullptr) {
      t->elem = findMin(t->right);
      removed =
          remove(t->right, t->elem); // Must return true, since already in tree
    } else {                         // Case of one child or no children
      AVLNode *old = t;
      t = (t->left != nullptr ? t->left : t->right);
      delete old;
      removed = true;
    }

    apply_rotation(t);
    return removed;
  }

  static void printTree(std::ostream &out, AVLNode *tree, std::string prefix,
                        bool isLeft) {
    if (!tree)
      return;

    out << prefix;
    out << (isLeft ? "├── " : "└── ");

    // Print current node
    out << tree->elem << " (h=" << tree->height << ")" << std::endl;

    // Enter the next tree level - left and right branch
    printTree(out, tree->left, prefix + (isLeft ? "│   " : "    "), true);
    printTree(out, tree->right, prefix + (isLeft ? "│   " : "    "), false);
  }

private:
  AVLNode *_root;
};

template <typename T>
std::ostream &operator<<(std::ostream &out, const AVLTree<T> &tree) {
  if (tree.empty()) {
    out << "Empty Tree" << std::endl;
  } else {
    out << tree._root->elem << " (h=" << tree._root->height << ")" << std::endl;
    AVLTree<T>::printTree(out, tree._root->left, "", true);
    AVLTree<T>::printTree(out, tree._root->right, "", false);
  }
  return out;
}
