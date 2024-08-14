#pragma once

#include "UniformRandom.h"
#include <algorithm>
#include <string>
#include <utility>
#include <vector>

int nextPrime(int n);

template <typename T> class CuckooHashFamily {
public:
  virtual std::size_t hash(const T &x, int which) const = 0;
  virtual int getNumberOfFunctions() = 0;
  virtual void generateNewFunctions() = 0;
};

template <int count>
class StringHashFamily : public CuckooHashFamily<std::string> {
public:
  StringHashFamily() : MULTIPLIERS(count) { generateNewFunctions(); }
  int getNumberOfFunctions() const { return count; }
  void generateNewFunctions() {
    for (auto &mult : MULTIPLIERS)
      mult = r.nextInt();
  }

  size_t hash(const std::string &x, int which) const {
    const int mult = MULTIPLIERS[which];
    size_t hash_val = 0;

    for (auto ch : x)
      hash_val = mult * hash_val + ch;

    return hash_val;
  }

private:
  vector<int> MULTIPLIERS;
  UniformRandom r;
};

template <typename T, typename HashFamily> class HashTable {
public:
  explicit HashTable(int size = 101) : array(nextPrime(size)) {
    numHashFuncitons = hashFunctions.getNumberOfFunctions();
    rehashes = 0;
    makeEmpty();
  }

  bool contains(const T &x) const { return findPos(x) != -1; }

  void makeEmpty() {
    current_size = 0;
    for (auto &elem : array)
      elem.isActive = false;
  }

  bool insert(const T &x) {
    if (contains(x))
      return false;
    if (current_size >= array.size() * LOAD_FACTOR)
      expand();
    return insertHelper1(x);
  }

  bool insert(T &&x) {
    if (contains(x))
      return false;
    if (current_size >= array.size() * LOAD_FACTOR)
      expand();
    return insertHelper1(std::move(x));
  }

  int size() const { return current_size; }

  int capacity() const { return array.size(); }

  bool remove(const T &x) {
    int current_pos = findPos(x);
    if (!isActive(current_pos))
      return false;

    array[current_pos].isActive = false;
    return true;
  }

private:
  struct HashEntry {
    T element;
    bool isActive;
    HashEntry(const T &e = T(), bool a = false) : element{e}, isActive{a} {}
    HashEntry(T &&e = T(), bool a = false)
        : element{std::move(e)}, isActive{a} {}
  };

  vector<HashEntry> array;
  int current_size;
  int numHashFuncitons;
  int rehashes;
  UniformRandom r;
  HashFamily hashFunctions;

  static constexpr double LOAD_FACTOR = 0.4;
  static constexpr double ALLOWED_REHASHES = 5;

  bool insertHelper1(const T &xx) {
    constexpr int COUNT_LIMIT = 100;
    T x = xx;
    // This will do multiple iterations if no return is executed, that is, when
    // all hash function positions are active Then we find a position not equal
    // to -1 and swaps the value and try to look for a position for that
    // position
    while (true) {
      int lastPos = -1;
      int pos;
      for (int count = 0; count < COUNT_LIMIT; ++count) {
        for (int i = 0; i < numHashFuncitons; ++i) {
          pos = myhash(x, i);
          if (!isActive(pos)) {
            array[pos] = std::move(HashEntry{std::move(x), true});
            ++current_size;
            return true;
          }
        }

        // None is found
        int i = 0;
        do {
          pos = myhash(x, r.nextInt(numHashFuncitons));
        } while (pos == lastPos && i++ < 5);

        lastPos = pos;
        std::swap(x, array[pos].element);
      }

      if (++rehashes > ALLOWED_REHASHES) {
        expand(); // make the table bigger
        rehashes = 0;
      } else {
        rehash(); // rehash calls insert, which is implicit recursion
      }
    }
  }

  bool isActive(int current_pos) const {
    return current_pos != -1 && array[current_pos].isActive;
  }

  int findPos(const T &x) const {
    for (int i = 0; i < numHashFuncitons; ++i) {
      int pos = myhash(x, i);
      if (isActive(pos) && array[pos].element == x)
        return pos;
    }

    return -1;
  }

  void expand() { rehash(static_cast<int>(array.size() / LOAD_FACTOR)); }

  void rehash() {
    hashFunctions.generateNewFunctions();
    rehash(array.size());
  }

  void rehash(int new_size) {
    vector<HashEntry> old = array; // copy

    array.resize(nextPrime(new_size));
    makeEmpty();

    for (auto &entry : old)
      if (entry.isActive)
        insert(std::move(entry.element));
  }

  size_t myhash(const T &x, int which) const {
    return hashFunctions.hash(x, which) % array.size();
  }
};
