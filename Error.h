#pragma once
#include <exception>
#include <string>
class Error : public std::exception {
public:
  Error(const char *mesg) : message{mesg} {}
  Error(std::string mesg) : message{mesg} {}

  const char *what() const throw() { return message.c_str(); }

private:
  std::string message;
};
