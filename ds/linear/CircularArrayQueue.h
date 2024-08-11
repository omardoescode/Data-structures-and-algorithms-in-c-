#pragma once

#include "../../Error.h"
#include <cstddef>
#include <utility>

/*
 * Invariants for impelementation:
 *   _back: stores the index at which the next element would be pushed
 *   _front: stores the index of the front element
 */

template <typename Object> class queue {
public:
  explicit queue(std::size_t capacity = 0)
      : _capacity(std::max(capacity, SPARE_CAPACITY)), _size(0), _front(0),
        _back(0) {
    _arr = new Object[_capacity];
  }

  queue(const queue &rhs) {
    _arr = new Object[rhs.capacity()];

    copy_to_first_idx(_arr, rhs);

    _front = 0;
    _back = rhs.size();
  }

  queue(queue &&rhs)
      : _arr(rhs._arr), _front(rhs._front), _back(rhs._back), _size(rhs._size),
        _capacity(rhs._capacity) {
    rhs._arr = nullptr;
    rhs._front = rhs._back = rhs._size = rhs._capacity = 0;
  }

  queue &operator=(const queue &rhs) {
    // avoid copying the same object
    if (this == &rhs)
      return *this;

    auto copy = rhs;
    std::swap(copy, *this);
    return *this;
  }

  queue &operator=(queue &&rhs) {
    if (this == &rhs)
      return *this;

    std::swap(_arr, rhs._arr);
    std::swap(_front, rhs._front);
    std::swap(_back, rhs._back);
    std::swap(_size, rhs._size);
    std::swap(_capacity, rhs._capacity);
    return *this;
  }

  ~queue() {
    delete[] _arr;
    _arr = nullptr;
  }

  std::size_t size() const { return _size; }
  std::size_t capacity() const { return _capacity; }
  bool empty() const { return _size == 0; }

  const Object &front() const { return _arr[_front]; }

  void push(const Object &value) {
    if (_size == _capacity) {
      resize();
      push(value);
      return;
    }

    _size++;
    _arr[_back] = value;
    _back = (_back + 1) % capacity();
  }

  void push(Object &&value) {
    if (_size == _capacity) {
      resize();
      push(std::move(value));
      return;
    }

    _size++;
    _arr[_back] = std::move(value);
    _back = (_back + 1) % capacity();
  }

  void pop() {
    if (empty())
      throw Error("Invalid popping: queue is empty");
    _front = (_front + 1) % capacity();
    _size--;
  }

private:
  void resize() { reserve(std::max(SPARE_CAPACITY, 2 * capacity() + 1)); }

  void reserve(std::size_t new_capacity) {
    if (new_capacity < capacity())
      return;

    Object *temp = new Object[new_capacity];
    copy_to_first_idx(temp, *this);
    std::swap(temp, _arr);
    delete[] temp;

    _front = 0;
    _back = _size;
    _capacity = new_capacity;
  }

  static void copy_to_first_idx(Object *to, queue<Object> &other) {
    for (int j = 0, i = other._front; j < other.size();
         j++, i = (i + 1) % other.capacity())
      to[j] = other._arr[i];
  }

private:
  Object *_arr;
  std::size_t _front;
  std::size_t _back;
  std::size_t _size, _capacity;

  static constexpr std::size_t SPARE_CAPACITY = 20;
};
