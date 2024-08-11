#include "../ds/linear/stack.h"
#include "../ds/trees/BinaryTree.h"
#include <cctype>

BinaryNode<char> construct_expression(std::string &msg) {
  stack<BinaryNode<char> *> values;

  for (auto &c : msg) {
    if (c == ' ')
      continue;

    if (isalnum(c)) {
      auto node = new BinaryNode<char>(c);
      std::cout << "pushing " << c << std::endl;
      values.push(node);
    } else {
      std::cout << "popping and pushing " << c << std::endl;
      auto op1 = values.top();
      values.pop();
      auto op2 = values.top();
      values.pop();

      auto node = new BinaryNode<char>(c, op1, op2);
      values.push(node);
    }
  }

  return *values.top();
}
