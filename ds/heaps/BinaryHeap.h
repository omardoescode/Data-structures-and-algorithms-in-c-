#pragma once

#include "../../Error.h"
#include <cmath>
#include <vector>
template <typename Comparable> class BinaryHeap {
public:
  explicit BinaryHeap(int capacity = DEFAULT_CAPACITY)
      : array(capacity + 1), current_size(0) {}
  explicit BinaryHeap(const std::vector<Comparable> &items)
      : array(items.size() + 10, current_size(items.size)) {
    for (int i = 0; i < items.size(); i++)
      array[i + 1] = items[i];
    buildHeap();
  }

  bool isEmpty() const { return current_size == 0; }
  const Comparable &findMin() const {
    if (isEmpty())
      return Error("Underflow");

    return array[1];
  }

  void insert(const Comparable &x) {
    if (current_size == array.size() - 1)
      array.resize(array.size() * 2);

    int hole = ++current_size;
    Comparable copy = x;

    array[0] = std::move(copy);

    for (; x < array[hole / 2]; hole /= 2)
      array[hole] = std::move(array[hole / 2]);

    array[hole] = array[0];
  }
  void insert(Comparable &&x) {

    if (current_size == array.size() - 1)
      array.resize(array.size() * 2);

    int hole = ++current_size;
    for (; x < array[hole / 2]; hole /= 2)
      array[hole] = std::move(array[hole / 2]);
    array[hole] = std::move(x);
  }

  void deleteMin() {
    if (isEmpty()) {
      throw Error("UnderFlow");
    }
    array[1] = std::move(array[current_size--]);
    percolateDown(1);
  }

  // save the value at minItem
  void deleteMin(Comparable &minItem) {
    if (isEmpty())
      throw Error{"Underflow"};

    minItem = std::move(array[1]);
    array[1] = std::move(array[current_size--]);
    percolateDown(1);
  }
  void makeEmpty() { current_size = 0; }

private:
  static constexpr int DEFAULT_CAPACITY = 100;
  int current_size;
  std::vector<Comparable> array;

  void buildHeap() {
    for (int i = current_size / 2; i > 0; --i)
      percolateDown(i);
  }
  void percolateDown(int hole) {
    int child;
    Comparable temp = std::move(array[hole]);

    for (; hole * 2 <= current_size; hole = child) {
      child = hole * 2;
      // see if need to go to right node
      if (child != current_size && array[child + 1] < array[child])
        child++;

      // Check if we can go furhter down
      if (array[child] < temp)
        array[hole] = std::move(array[child]);
      else // done
        break;
    }

    array[hole] = std::move(temp);
  }

  int get_parent(int i) { return floor(i); }
  int get_left_child(int i) { return 2 * i; }
  int get_right_child(int i) { return 2 * i + 1; }
};
