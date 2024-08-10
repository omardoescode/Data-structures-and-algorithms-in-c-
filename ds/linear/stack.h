#pragma once

#include "vector.h"

template <typename Object> class stack {
public:
  stack() = default;

  stack(const stack &rhs) {
    _elems.reserve(rhs.size());
    for (auto &value : rhs._elems) {
      _elems.push_back(value);
    }
  }

  stack &operator=(const stack &rhs) {
    auto copy = rhs;
    std::swap(copy, *this);
    return *this;
  }

  stack(stack &&rhs) : _elems(rhs._elems) { rhs._elems.clear(); }

  stack &operator=(stack &&rhs) {
    std::swap(_elems, rhs._elems);
    return *this;
  }

  size_t size() const { return _elems.size(); }
  bool empty() const { return size() == 0; }

  Object top() const {
    if (size() == 0)
      throw Error("Stack is empty");

    return _elems.back();
  }

  void pop() { _elems.pop_back(); }

  void push(const Object &value) { _elems.push_back(value); }
  void push_top(Object &&value) { _elems.push_back(std::move(value)); }

private:
  vector<Object> _elems;
};
