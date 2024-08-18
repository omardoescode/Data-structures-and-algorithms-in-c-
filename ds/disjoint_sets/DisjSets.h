#pragma once

#include <numeric>
#include <vector>
class DisjSets {
public:
  explicit DisjSets(int numElements) : s{numElements, -1} {}

  int find(int x) const {
    if (s[x] < 0)
      return x;
    else
      return find(s[x]);
  }
  int find(int x) {
    if (s[x] < 0)
      return x;
    else
      return s[x] = find(s[x]); // path compression
  }
  void unionSets(int root1, int root2) { s[root2] = root1; }

  int groups() const {
    return std::accumulate(s.begin(), s.end(), 0,
                           [](int &a, int &b) { return a + (b == -1); });
  }

private:
  std::vector<int> s;
};
