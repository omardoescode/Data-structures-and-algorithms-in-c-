#include <string>
#include <vector>

// Radix sort for strings
void radixSortA(std::vector<std::string> &arr, int stringLen) {
  constexpr int BUCKETS = 256;
  std::vector<std::vector<std::string>> buckets(BUCKETS);

  for (int pos = stringLen - 1; pos >= 0; --pos) {
    // Fill the buckets by the character at position `pos`
    for (auto &s : arr)
      buckets[s[pos]].push_back(std::move(s));

    // dump everything back to arr
    int idx = 0;
    for (auto &bucket : buckets) {
      for (auto &s : bucket)
        arr[idx++] = std::move(s);

      bucket.clear();
    }
  }
}

// Couting Radix Sort for strings
void countingRadixSort(std::vector<std::string> &arr, int stringLen) {
  constexpr int BUCKETS = 256;

  int N = arr.size();
  std::vector<std::string> buffer(N);

  auto *in = &arr;
  auto *out = &buffer;

  for (int pos = stringLen - 1; pos >= 0; --pos) {
    std::vector<int> count(BUCKETS + 1);
    for (int i = 0; i < N; ++i) {
      ++count[(*in)[i][pos] + 1];
    }
    for (int b = 0; b < BUCKETS; ++b) {
      count[b] += count[b - 1];
    }

    for (int i = 0; i < N; ++i)
      (*out)[count[(*in)[i][pos]++]] = std::move((*in)[i]);

    std::swap(in, out);
  }

  // We swap in and out every loop, so if we got an odd number of passes, arr is
  // not referencing the result
  if (stringLen % 2 == 1)
    for (int i = 0; i < arr.size(); ++i)
      (*out)[i] = std::move((*in)[i]);
}

void radixSort(std::vector<std::string> &arr, int maxLen) {
  constexpr int BUCKETS = 256;

  std::vector<std::vector<std::string>> wordsByLength(maxLen + 1);
  std::vector<std::vector<std::string>> buckets(BUCKETS);

  // Sort the strings by length
  for (auto &s : arr)
    wordsByLength[s.length()].push_back(std::move(s));

  int idx = 0;
  for (auto &wordList : wordsByLength)
    for (auto &s : wordList)
      arr[idx++] = std::move(s);

  int start = arr.size();
  for (int pos = maxLen - 1; pos >= 0; --pos) {
    start -= wordsByLength[pos + 1].size();

    for (int i = 0; i < arr.size(); ++i) {
      buckets[arr[i][pos]].push_back(std::move(arr[i]));
    }

    idx = start;
    for (auto &buck : buckets) {
      for (auto &s : buck)
        arr[idx++] = std::move(s);

      buck.clear();
    }
  }
}
