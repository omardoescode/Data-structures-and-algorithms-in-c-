#include "../ds/linear/DoubleLinkedList.h"
#include <iostream>
/*
 * You are given a list, L, and another list, P, containing integers sorted in
 * ascending order. The operation printLots(L,P) will print the elements in L
 * that are in positions specified by P. For instance, if P = 1, 3, 4, 6, the
 * elements in positions 1, 3, 4, and 6 in L are printed. Write the procedure
 * printLots(L,P). You may use only the public STL container operations. What is
 * the running time of your procedure?
 *
 * zero indexed
 */

template <typename T> void printLots(const list<T> &l, const list<int> &p) {
  auto p_itr = p.begin();
  auto l_itr = l.begin();
  int last = 0;
  while (p_itr != p.end()) {
    for (int i = 0; i < *p_itr - last; i++)
      l_itr++;

    std::cout << *l_itr << std::endl;
    last = *p_itr;
    p_itr++;
  }
}

template <typename T>
void intersection(const list<T> &l1, const list<T> &l2, list<T> &result) {
  auto itr1 = l1.begin();
  auto itr2 = l2.begin();

  while (itr1 != l1.end()) {

    while (itr2 != l2.end() && *itr2 < *itr1) {
      itr2++;
    }
    if (*itr1 == *itr2) {
      result.push_back(*itr1);
    }

    itr1++;
  }
}
