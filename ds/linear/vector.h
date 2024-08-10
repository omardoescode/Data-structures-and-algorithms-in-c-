#pragma once

#include "../../Error.h"
#include <algorithm>
#include <initializer_list>
#include <iostream>
#include <utility>

// WARN: no type checking
template <typename Object> class vector {
public:
  explicit vector(size_t init_size = 0)
      : _size{init_size}, _capacity{init_size + SPARE_CAPACITY} {
    _objects = new Object[_capacity];
  }

  vector(std::initializer_list<Object> elems) : vector() {
    reserve(elems.size());
    for (auto &elem : elems) {
      push_back(elem);
    }
  }

  vector(const vector &rhs)
      : _size(rhs._size), _capacity(rhs._capacity), _objects(nullptr) {
    _objects = new Object[_capacity];
    for (int i = 0; i < _size; i++)
      _objects[i] = rhs._objects[i];
  }

  vector &operator=(const vector &rhs) {
    vector copy = rhs;
    std::swap(*this, copy);
    return *this;
  }

  ~vector() { delete[] _objects; }

  vector(vector &&rhs)
      : _size(rhs._size), _capacity(rhs._capacity), _objects(rhs._objects) {
    rhs._objects = nullptr;
    rhs._size = 0;
    rhs._capacity = 0;
  }

  vector &operator=(vector &&rhs) {
    swap(_size, rhs._size);
    swap(_capacity, rhs._capacity);
    swap(_objects, rhs._objects);
  }

  Object &operator[](int index) { return _objects[index]; }
  const Object &operator[](int index) const { return _objects[index]; }

  void resize(int new_size) {
    if (new_size > _capacity)
      reserve(new_size * 2);
    _size = new_size;
  }

  void reserve(int new_capacity) {
    if (new_capacity < _size)
      return;

    Object *new_array = new Object[new_capacity];

    for (int k = 0; k < _size; ++k)
      new_array[k] = std::move(_objects[k]);

    _capacity = new_capacity;
    std::swap(_objects, new_array);
    delete[] new_array;
  }

  bool empty() const { return size() == 0; }
  size_t size() const { return _size; }
  size_t capacity() const { return _capacity; }

  void push_back(const Object &x) {
    if (_size == _capacity)
      reserve(2 * _capacity + 1);
    _objects[_size++] = x;
  }

  void push_back(Object &&x) {
    if (_size == _capacity)
      reserve(2 * _capacity + 1);
    _objects[_size++] = std::move(x);
  }

  void pop_back() { _size--; }

  Object front() {
    if (size() == 0)
      throw Error("Vector is empty");
    return _objects[0];
  }
  Object &front() const {
    if (size() == 0)
      throw Error("Vector is empty");
    return _objects[0];
  }
  Object back() {
    if (size() == 0)
      throw Error("Vector is empty");
    return _objects[size() - 1];
  }
  Object &back() const {
    if (size() == 0)
      throw Error("Vector is empty");
    return _objects[size() - 1];
  }

  typedef Object *iterator;
  typedef const Object *const_iterator;

  iterator begin() { return &_objects[0]; }
  const_iterator begin() const { return &_objects[0]; }

  iterator end() { return &_objects[_size]; }
  const_iterator end() const { return &_objects[_size]; }

private:
  size_t _size, _capacity;
  Object *_objects;
  constexpr static size_t SPARE_CAPACITY = 16;
};
