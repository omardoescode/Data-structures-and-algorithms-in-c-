#include <vector>
template <typename Comparable> void quicksort(std::vector<Comparable> &items) {
  if (items.size() <= 1)
    return; // base case

  std::vector<Comparable> smaller;
  std::vector<Comparable> same;
  std::vector<Comparable> larger;

  auto chosenItem = items[items.size() / 2];

  for (auto &i : items) {
    if (i < chosenItem)
      smaller.push_back(std::move(i));
    else if (i == chosenItem)
      same.push_back(std::move(i));
    else
      larger.push_back(std::move(i));
  }

  quicksort(smaller);
  quicksort(larger);

  std::move(begin(smaller), end(smaller), begin(items));
  std::move(begin(same), end(same), begin(items) + smaller.size());
  std::move(begin(larger), end(larger), end(items) + larger.size());
}
