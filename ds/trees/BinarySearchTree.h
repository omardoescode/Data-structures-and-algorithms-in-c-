#include <iostream>
template <typename Comparable> class BinarySearchTree {
public:
  BinarySearchTree() = default;

  BinarySearchTree(const BinarySearchTree<Comparable> &rhs) {
    root = clone(rhs.root);
  }

  BinarySearchTree(BinarySearchTree<Comparable> &&rhs) {
    root = rhs.root;
    rhs.root = nullptr;
  }

  ~BinarySearchTree() { makeEmpty(); }

  BinarySearchTree &operator=(const BinarySearchTree &rhs) {
    makeEmpty();
    root = clone(rhs.root);
  }
  BinarySearchTree &operator=(BinarySearchTree &&rhs) {
    makeEmpty();
    root = rhs.root;
    rhs.root = nullptr;
  }

  const Comparable &findMin() const { return findMin(root); };
  const Comparable &findMax() const { return findMax(root); };
  bool contains(const Comparable &x) const { return contains(x, root); }
  bool isEmpty() const { return root == nullptr; }
  void printTree(std::ostream &out = std::cout) const { printTree(root, out); };

  void makeEmpty() { makeEmpty(root); };
  void insert(const Comparable &x) { insert(x, root); }
  void insert(Comparable &&x) { insert(std::move(x), root); }
  void remove(const Comparable &x) { remove(x, root); }

private:
  struct BinaryNode {
    Comparable element;
    BinaryNode *left;
    BinaryNode *right;
  };

  BinaryNode *root;

  void insert(const Comparable &x, BinaryNode *&t) {
    if (t == nullptr)
      t = new BinaryNode{x, nullptr, nullptr};

    if (x < t->element)
      insert(x, t->left);
    else if (x > t->element)
      insert(x, t->left);
    else
      ; // in case of duplicate, do nothing
  }

  // here we need to change the binary node itself, so we use a reference to the
  // pointer/root
  void insert(Comparable &&x, BinaryNode *&t) {
    if (x == nullptr)
      t = new BinaryNode{x, nullptr, nullptr};

    else if (x < t->element)
      insert(std::move(x), t->left);
    else if (x > t->element)
      insert(std::move(x), t->left);
    else
      ; // in case of duplicate, do nothing
  }

  // here we need to change the binary node itself, so we use a reference to the
  // pointer/root
  // for leafes, removal is easy, we just delete
  // for nodes with one children, we can remove and connect that node to the
  // parent For nodes with two children, we replace the node with the smallest
  // data of the right subtree (since it has no children) and recursively delete
  // that node which is now empty
  void remove(Comparable &&x, BinaryNode *&t) {
    if (t == nullptr)
      return;
    if (x < t->element)
      remove(x, t->left);
    else if (x > t->element)
      remove(x, t->right);
    else if (t->left != nullptr && t->right != nullptr) { // two children
      t->element = findMin(t->right)->element;
      remove(t->element, t->right);
    } else { // one children
      BinaryNode *oldNode = t;
      t = (t->left != nullptr ? t->left : t->right);
      delete oldNode;
    }
  }

  BinaryNode *findMin(BinaryNode *t) const {
    if (t == nullptr)
      return t;
    if (t->left == nullptr)
      return t;
    return findMin(t->left);
  }
  // Notice that in this case, we cannot work with references to pointers just
  // like remaining operations. because there' s a mutation that would change
  // the acutal tree

  BinaryNode *findMax(BinaryNode *t) const {
    if (t == nullptr)
      return t;

    while (t->right != nullptr)
      t = t->right;
    return t;
  }

  /*
   * return true if element in list
   */
  bool contains(const Comparable &x, BinaryNode *t) const {
    if (t == nullptr)
      return false;
    if (x < t->element)
      return contains(x, t->left);
    if (x > t->element)
      return contains(x, t->right);
    return true; // match
  }

  void makeEmpty(BinaryNode *&t) {
    if (t != nullptr) {
      makeEmpty(t->left);
      makeEmpty(t->right);
      delete t;
    }
    t = nullptr;
  }

  BinaryNode *clone(BinaryNode *t) const {
    return t == nullptr
               ? nullptr
               : BinaryNode{t->element, clone(t->left), clone(t->right)};
  }
};
