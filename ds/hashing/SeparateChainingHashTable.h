#include "../linear/DoubleLinkedList.h"
#include "../linear/vector.h"

template <typename HashedObj> class HashTable {
public:
  explicit HashTable(int size = 101) : current_size{0} { lists.resize(101); }

  const int size() const { return current_size; }
  bool contains(const HashedObj &x) const {
    auto &whichlist = lists[myhash(x)];
    return find(begin(whichlist), end(whichlist), x) != end(whichlist);
  }

  void makeEmpty() {
    current_size = 0;
    for (auto &list : lists)
      list.clear();
  }

  bool insert(const HashedObj &x) {
    auto &whichlist = lists[myhash(x)];
    if (find(begin(whichlist), end(whichlist), x) == end(whichlist))
      return false;

    whichlist.push_back(x);

    if (++current_size > lists.size())
      rehash();

    return true;
  }

  bool insert(HashedObj &&x);

  bool remove(const HashedObj &x) {
    auto &whichlist = lists[myhash(x)];
    auto itr = find(begin(whichlist), end(whichlist), x);

    if (itr == end(whichlist))
      return false;

    whichlist.erase(itr);
    current_size--;
    return true;
  }

private:
  vector<list<HashedObj>> lists;
  int current_size;

  void rehash() {
    vector<list<HashedObj>> old = lists; // copy

    lists.resize(nextPrime(2 * old.size()));
    makeEmpty();

    for (auto &list : old)
      for (auto &x : list)
        insert(std::move(x));
  }

  size_t myhash(const HashedObj &x) const {
    static hash<HashedObj> hf;
    return hf(x) % lists.size();
  }
};
}
;
