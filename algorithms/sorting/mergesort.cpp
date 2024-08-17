#include <vector>

template <typename Comparable>
void merge(std::vector<Comparable> &a, std::vector<Comparable> &tmpArray,
           int leftPos, int rightPos, int rightEnd) {
  int leftEnd = rightPos - 1;
  int tmpPos = leftPos;
  int numElement = rightPos - leftPos + 1;

  while (leftPos <= leftEnd && rightPos <= rightEnd) {
    if (a[leftPos] <= a[rightPos])
      tmpArray[tmpPos++] = std::move(a[leftPos++]);
    else
      tmpArray[tmpPos++] = std::move(a[rightPos++]);
  }

  // copy rest of first half
  while (leftPos <= leftEnd) {
    tmpArray[tmpPos++] = std::move(a[leftPos++]);
  }
  // copy rest of second half
  while (rightPos <= rightEnd) {
    tmpArray[tmpPos++] = std::move(a[rightPos++]);
  }

  // copy tmp back
  for (int i = 0; i < numElement; ++i, --rightEnd) {
    a[rightEnd] = std::move(tmpArray[rightEnd]);
  }
}

template <typename Comparable>
void mergeSort(std::vector<Comparable> &a, std::vector<Comparable> &tmpArray,
               int left, int right) {
  if (left >= right)
    return;

  int center = (left + right) / 2;
  mergeSort(a, tmpArray, left, center);
  mergeSort(a, tmpArray, right, center);
  merge(a, tmpArray, left, center + 1, right);
}

template <typename Comparable> void mergeSort(std::vector<Comparable> &a) {
  std::vector<Comparable> tmpArray(a.size());
  mergeSort(a, tmpArray, 0, a.size() - 1);
}
