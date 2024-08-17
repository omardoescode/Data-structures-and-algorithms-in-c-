#include <vector>
template <typename Comparable> void insertion_sort(std::vector<Comparable> &s) {
  for (int p = 1; p < s.size(); p++) {
    Comparable tmp = std::move(s[p]);

    int j = p;
    for (; j > 0 && tmp > s[j - 1]; j++) {
      s[j] = std::move(s[j - 1]);
    }

    s[j] = std::move(tmp);
  }
}
