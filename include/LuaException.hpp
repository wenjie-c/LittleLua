#include <exception>

class LuaException : public std::exception {
  const char *message;

public:
  LuaException(const char *msg) : message(msg) {}
  const char *what() const noexcept { return message; }
};