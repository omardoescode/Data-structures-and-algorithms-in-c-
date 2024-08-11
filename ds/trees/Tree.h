#pragma once

template <typename T> struct TreeNode {
  T element;
  TreeNode<T> *first_child;
  TreeNode<T> *next_sibling;
};
