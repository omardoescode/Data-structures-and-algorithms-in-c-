#include <utility>

// TODO: Complete implmenetation by coping from binary search tree
template <typename Comparable> class AvlTree {
private:
  static constexpr int ALLOWED_IMBALANCE = 1;
  struct AvlNode {
    Comparable element;
    AvlNode *left;
    AvlNode *right;
    int height;

    AvlNode(const Comparable &ele, AvlNode *lt, AvlNode *rt, int h = 0)
        : element(ele), left{lt}, right{rt}, height(h) {}
    AvlNode(Comparable &&ele, AvlNode *lt, AvlNode *rt, int h = 0)
        : element(std::move(ele)), left{lt}, right{rt}, height(h) {}
  };

public:
  AvlTree() = default;

  int height(AvlNode *t) const { return t == nullptr ? -1 : t->height; }

  void insert(const Comparable &object) { insert(_root, object); }
  void insert(Comparable &&object) { insert(_root, std::move(object)); }

private:
  void insert(AvlNode *t, const Comparable &object) {
    if (t == nullptr)
      t = {object, nullptr, nullptr};
    else if (object > t->element)
      insert(t->right, object);
    insert(t->left, object);
  }
  void insert(AvlNode *t, Comparable &&object) {
    if (t == nullptr)
      t = {std::move(object), nullptr, nullptr};
    else if (object > t->element)
      insert(t->right, std::move(object));
    insert(t->left, std::move(object));

    apply_rotation(t);
  }

  int calculate_balance(AvlNode *t) {
    if (t == nullptr)
      return 0;
    return height(t->left) - height(t->right);
  }
  void apply_rotation(AvlNode *t) {
    if (t == nullptr)
      return;

    int balance = calculate_balance(t);
    if (balance > ALLOWED_IMBALANCE)
      if (height(t->left->left) >= height(t->left->right))
        rotateWithLeftChild(t);
      else
        doubleRotateWithLeftChild(t);
    else if (balance < -ALLOWED_IMBALANCE)
      if (height(t->right->right) >= height(t->right->left))
        rotateWithRightChild(t);
      else
        doubleRotateWithRightChild(t);

    t->height = max(height(t->left), height(t->right)) + 1;
  }

  void rotateWithLeftChild(AvlNode *&k2) {
    AvlNode *k1 = k2->left;
    k2->left = k1->right;
    k1->right = k2;
    k2->height = max(height(k2->left), height(k2->right)) + 1;
    k1->height = max(height(k1->left), height(k1->right)) + 1;
    k2 = k1;
  }

  void rotateWithRightChild(AvlNode *&k1) {
    AvlNode *k2 = k1->right;
    k1->right = k2->left;
    k2->left = k1;
    k1->height = max(height(k1->left), height(k1->right)) + 1;
    k2->height = max(height(k2->right), height(k2->height)) + 1;
    k1 = k2;
  }

  void doubleRotateWithLeftChild(AvlNode *&k3) {
    rotateWithRightChild(k3->left);
    rotateWithLeftChild(k3);
  }
  void doubleRotateWithRightChild(AvlNode *&k3) {
    rotateWithLeftChild(k3->right);
    rotateWithRightChild(k3);
  }

  void remove(const Comparable &x, AvlNode *&t) {
    if (t == nullptr)
      return;

    if (x < t->element)
      remove(x, t->left);
    else if (x > t->element)
      remove(x, t->right);
    else if (t->left != nullptr && t->right != nullptr) {
      t->element = findMin(t->right)->element;
      remove(t->element, t->right);
    } else {
      AvlNode *oldNode = t;
      t = (t->left != nullptr) ? t->left : t->right;
      delete oldNode;
    }

    apply_rotation(t);
  }

private:
  AvlNode *_root;
};
