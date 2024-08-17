#include <functional>
template <typename Iterator, typename Comparator>
void insertion_sort(const Iterator &begin, const Iterator &end,
                    Comparator lessThan) {
  if (begin == end)
    return;

  Iterator j;

  for (Iterator p = begin + 1; p != end; ++p) {
    auto tmp = std::move(*p);

    for (j = p; j != begin && lessThan(tmp, *(j - 1)); --j)
      *j = std::move((*j - 1));

    *j = std::move(tmp);
  }
}

template <typename Iterator>
void insertion_sort(const Iterator &begin, const Iterator &end) {
  insertion_sort(begin, end, std::less<decltype(*begin)>{});
}
