#include "../linear/vector.h"
#include <cctype>
#include <utility>

int nextPrime(int n); // define somewhere else to have coupling

template <typename HashedObj> class HashTable {
public:
  explicit HashTable(int size = 101) : array(nextPrime(size)), current_size(0) {
    make_empty();
  }

  bool contains(const HashedObj &x) { return isActive(findPos(x)); }
  void make_empty() {
    current_size = 0;
    for (auto &entry : array)
      entry.info = EMPTY;
  }

  bool insert(const HashedObj &x) {
    int currentPos = findPos(x);
    if (isActive(currentPos))
      return false;

    if (array[currentPos].info != DELETED)
      ++current_size;

    array[currentPos].element = x;
    array[currentPos].info = ACTIVE;

    if (current_size > array.size() * LOAD_FACTOR)
      rehash();

    return true;
  }

  bool insert(HashedObj &&x) {
    int currentPos = findPos(x);
    if (isActive(currentPos))
      return false;

    if (array[currentPos].info != DELETED)
      ++current_size;

    array[currentPos].element = std::move(x);
    array[currentPos].info = ACTIVE;

    if (current_size > array.size() * LOAD_FACTOR)
      rehash();

    return true;
  }

  bool remove(const HashedObj &x) {
    int currentPos = findPos(x);
    if (!isActive(currentPos))
      return false;

    array[currentPos].info = DELETED;
    return true;
  }

private:
  enum EntryType { ACTIVE, EMPTY, DELETED };
  struct HashEntry {
    HashedObj element;
    EntryType info;

    HashEntry(const HashedObj &e = HashedObj{}, EntryType i = EMPTY)
        : element{e}, info{i} {}
    HashEntry(HashedObj &&e = HashedObj{}, EntryType i = EMPTY)
        : element{std::move(e)}, info{i} {}
  };

  int current_size;
  vector<HashedObj> array;
  static constexpr double LOAD_FACTOR = .5;

  bool isActive(int currentPos) { return array[currentPos].info == ACTIVE; }

  int findPos(const HashedObj &x) const {
    // The easiest way to implement quadratic probing is by defnition: f(i) =
    // f(i - 1) + 2i - 1. the distance between the element increase by 2 easy
    // time starting with 1

    int offset = 1;
    int currentPos = myhash(x);

    // The order of tests here in the while loop is important. DON'T SWITCH
    while (array[currentPos].info != EMPTY && array[currentPos] != x) {
      currentPos = (currentPos + offset) % array.size();
      offset += 2;
    }

    return currentPos;
  }

  void rehash() {
    vector<HashEntry> oldArray = array; // copy

    array.resize(nextPrime(2 * oldArray.size()));
    make_empty();

    for (auto &entry : oldArray)
      if (entry.info == ACTIVE)
        insert(std::move(entry.element));
  }

  size_t myhash(const HashedObj &x) const {
    static hash<HashedObj> hf;
    return hf(x) % array.size();
  }
};
