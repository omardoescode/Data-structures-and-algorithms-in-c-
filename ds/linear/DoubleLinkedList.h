#pragma once
#include "../../Error.h"
#include <cstddef>
#include <exception>
#include <initializer_list>
#include <utility>

// TODO: Create the exceptions
// TODO: Complete the code to use the updated const_iterator

template <typename Object> class list {
private:
  // private nested struct
  // struct is used even to ease things and in addition, this is a private
  // nested struct so it cannot be accessed from outside the list so
  // implmentation is still sealed.
  struct node {
    Object _data;
    node *_prev;
    node *_next;

    node(const Object &d = Object(), node *p = nullptr, node *n = nullptr)
        : _data{d}, _prev{p}, _next{n} {}

    node(Object &&data, node *prev, node *next)
        : _data(std::move(data)), _prev(prev), _next(next) {}
  };

public:
  class const_iterator {
  public:
    const_iterator() : current{nullptr} {}
    const Object &operator*() const { return retreive(); }

    // itr++
    const_iterator &operator++() {
      current = current->_next;
      return *this;
    }

    // ++itr
    const_iterator operator++(int) {
      const_iterator old = *this;
      ++(*this);
      return old;
    }

    bool operator==(const const_iterator &rhs) const {
      return current == rhs.current;
    }

    bool operator!=(const const_iterator &rhs) const { return !(*this == rhs); }

  protected: // so iterator can access current
    const list<Object> *_list;
    node *current;

    Object &retreive() const { return current->_data; }

    // we want only list to access this constructor. only the friend list
    const_iterator(const list<Object> &lst, node *p)
        : current{p}, _list{&lst} {}

    void assert_is_valid() const {
      if (_list == nullptr || current == nullptr || current == _list->_head) {
        throw Error("Iterator out of bound");
      }
    }

    friend class list<Object>;
  };

  // It's the same as the const iterator except that operator* return a
  // reference rather than a constant reference
  // iterator is a constant reference, and this cannot be done the other way
  // around
  class iterator : public const_iterator {
  public:
    iterator() {}
    Object &operator*() { return const_iterator::retreive(); }
    const Object &operator*() const { return const_iterator::operator*(); }

    iterator &operator++() {
      // using this because in parent class
      this->current = this->current->_next;
      return *this;
    }

    iterator operator++(int) {
      iterator old = *this;
      ++(*this);
      return old;
    }

  protected:
    iterator(const list<Object> &lst, node *p) : const_iterator(lst, p) {}

    friend class list<Object>;
  };

public:
  list() { init(); }

  ~list() {
    clear();
    delete _head;
    delete _tail;
  }

  list(const std::initializer_list<Object> &elems) {
    init();

    for (const auto &val : elems)
      push_back(val);
  }

  list(const list &rhs) {
    init();
    for (auto &x : rhs)
      push_back(x);
  }

  /*
   * Use the copy constructor and swap instead of redoing the same this;
   */
  list &operator=(const list &rhs) {
    list copy = rhs;
    std::swap(*this, copy);
    return *this;
  }

  list(list &&rhs) : _size(rhs._size), _head(rhs._head), _tail(rhs._tail) {
    rhs._size = 0;
    rhs._head = rhs._tail = nullptr;
  }

  list &operator=(list &&rhs) {
    std::swap(_size, rhs._size);
    std::swap(_head, rhs._head);
    std::swap(_tail, rhs._tail);
  }

  iterator begin() { return {*this, _head->_next}; }
  const_iterator begin() const {
    const_iterator itr{*this, _head};
    return ++itr;
  }

  iterator end() { return {*this, _tail}; }
  const_iterator end() const { return {*this, _tail}; }

  size_t size() const { return _size; }
  bool empty() const { return size() == 0; }

  void clear() {
    while (!empty()) {
      pop_front();
    }
  }

  Object &front() { return *begin(); }
  const Object &front() const { return *begin(); }
  Object &back() { return *--end(); }
  const Object &back() const { return *--end(); }

  void push_back(const Object &x) { insert(end(), x); }
  void push_back(Object &&x) { insert(end(), std::move(x)); }

  void push_front(const Object &x) { insert(begin(), x); }
  void push_front(Object &&x) { insert(begin(), std::move(x)); }

  void pop_front() { erase(begin()); }

  void pop_back() { erase(--end()); }

  // insert it at the given iterator
  iterator insert(iterator itr, const Object &x) {
    itr.assert_is_valid();

    if (itr._list != this)
      throw Error("Invalid iterator");

    node *p = itr.current;
    _size++;
    // Create the node, connect it to the previois node and the node that it
    // replaced, adn then return it.
    return {*this, p->_prev = p->_prev->_next = new node{x, p->_prev, p}};
  }

  iterator insert(iterator itr, Object &&x) {
    node *p = itr.current;
    _size++;
    return {*this,
            p->_prev = p->_prev->_next = new node{std::move(x), p->_prev, p}};
  }

  iterator erase(iterator itr) {
    node *p = itr.current;
    iterator retVal(*this, p->_next);

    // make the node stale
    p->_prev->_next = p->_next;
    p->_next->_prev = p->_prev;
    delete p;
    _size--;

    return retVal;
  }

  iterator erase(iterator from, iterator to) {
    // we cannot use itr++ because it's stale after erasing
    // Instead update it by using the return value of the erase
    for (iterator itr = from; itr != to;)
      itr = erase(itr);

    return to;
  }

private:
  size_t _size;
  node *_head;
  node *_tail;

  // even though size is zero, we have two dummy nodes
  // Having dummy node as the header and the end of the element remove a host of
  // special cases, these nodes are called *sentinel nodes*
  void init() {
    _size = 0;
    _head = new node;
    _tail = new node;
    _head->_next = _tail;
    _tail->_prev = _head;
  }
};
