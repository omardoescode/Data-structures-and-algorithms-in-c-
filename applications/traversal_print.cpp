#pragma once
#include "../ds/trees/BinaryTree.h"
#include <iostream>

template <typename T> void preorder_print(BinaryNode<T> *tree) {
  if (tree == nullptr)
    return;
  std::cout << tree->value << " ";
  preorder_print(tree->left);
  preorder_print(tree->right);
}
template <typename T> void inorder_print(BinaryNode<T> *tree) {
  if (tree == nullptr)
    return;

  inorder_print(tree->left);
  std::cout << tree->value << " ";
  inorder_print(tree->right);
}
template <typename T> void postorder_print(BinaryNode<T> *tree) {
  if (tree == nullptr)
    return;

  postorder_print(tree->left);
  postorder_print(tree->right);
  std::cout << tree->value << " ";
}
