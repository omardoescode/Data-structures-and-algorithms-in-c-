#include <vector>
inline int leftChild(int i) { return 2 * i + 1; }

template <typename Comparable>
void percDown(std::vector<Comparable> &a, int i, int n) {
  int child;
  Comparable temp;

  for (temp = std::move(a[i]); leftChild(i) < n; i = child) {
    child = leftChild(i);
    if (child != n - 1 && a[child] < a[child + 1])
      ++child;
    if (temp < a[child])
      a[i] = std::move(a[child]);
    else
      break;
  }

  a[i] = std::move(temp);
}

template <typename Comparable> void heapsort(std::vector<Comparable> &a) {
  for (int i = a.size() / 2 - 1; i >= 0; --i) // build heap
    percDown(a, i, a.size());

  for (int j = a.size() - 1; j > 0; --j) {
    std::swap(a[0], a[j]); // deleteMax
    percDown(a, 0, j);
  }
}
