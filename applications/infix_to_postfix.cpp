#include "../ds/stack.h"
#include <algorithm>
#include <iostream>
#include <map>
#include <string>
int main() {
  std::string expression, output;
  getline(std::cin, expression);
  std::map<char, int> ops = {{'+', 1}, {'-', 1}, {'*', 2}, {'/', 2}};

  stack<char> st;
  int current_number = 0;
  bool set = false;
  for (auto &c : expression) {
    if (isdigit(c)) {
      current_number = current_number * 10 + (c - '0');
      set = true;
    } else if (std::any_of(ops.begin(), ops.end(),
                           [&](auto &value) { return value.first == c; })) {
      if (set) {
        output += std::to_string(current_number) + " ";
        set = false;
        current_number = 0;
      }

      if (st.empty())
        st.push(c);
      else if (c == '(')
        st.push(c);
      else if (c == ')') {
        while (st.top() != '(') {
          output += st.top();
          output += " ";
          st.pop();
        }
        if (!st.empty())
          st.pop();

      } else {
        while (!st.empty() && st.top() != '(' && ops[st.top()] >= ops[c]) {
          output += st.top();
          output += " ";
          st.pop();
        }

        st.push(c);
      }
    }
  }

  if (set) {
    output += std::to_string(current_number) + " ";
  }

  while (!st.empty()) {
    output += st.top();
    output += " ";
    st.pop();
  }

  std::cout << output << std::endl;
}
