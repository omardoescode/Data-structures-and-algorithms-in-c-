#pragma once

#include <utility>
template <typename T> struct BinaryNode {
  T value;
  BinaryNode<T> *left;
  BinaryNode<T> *right;

  BinaryNode() : value(), left(nullptr), right(nullptr) {}
  BinaryNode(const T &new_value, BinaryNode<T> *left = nullptr,
             BinaryNode<T> *right = nullptr)
      : value(new_value), left(left), right(right) {}

  BinaryNode(T &&new_value, BinaryNode<T> *left = nullptr,
             BinaryNode<T> *right = nullptr)
      : value(std::move(new_value)), left(left), right(right) {}
};
