#include <vector>

template <typename Comparable>
const Comparable &median3(std::vector<Comparable> &a, int left, int right) {
  int center = (left + right) / 2;
  if (a[center] < a[left])
    std::swap(a[left], a[center]);
  if (a[right] < a[left])
    std::swap(a[left], a[right]);
  if (a[right] < a[center])
    std::swap(a[center], a[right]);

  std::swap(a[center], a[right - 1]);
  return a[right - 1];
}

template <typename Comparable>
void quickSelect(std::vector<Comparable> &a, int left, int right, int k) {
  if (left + 10 > right)
    return insertionSort(a);

  const Comparable &pivot = median3(a, left, right);
  int i = left, j = right - 1;

  for (;;) {
    while (a[++i] < pivot)
      ;
    while (a[--j] > pivot)
      ;

    if (i < j)
      std::swap(a[i], a[j]);
    else
      break;
  }

  // restore
  std::swap(a[i], a[right - 1]);
  if (k <= i)
    quickSelect(a, left, i - 1, k);
  else
    quickSelect(a, i + 1, right, k);
}

template <typename Comparable>
void quickSelect(std::vector<Comparable> &a, int k) {
  quickSelect(a, 0, a.size() - 1, k);
}
