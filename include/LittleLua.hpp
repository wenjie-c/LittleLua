#include <LuaException.hpp>
#include <cstddef>
#include <iostream>
#include <string>
#include <vector>

extern "C" {
#include <lauxlib.h>
#include <lua.h>
#include <lualib.h>
}

#ifndef LittleLua_HPP
#define LittleLua_HPP

using namespace std;

typedef void(*Callback)(lua_State *);

class LittleLua {
public:
  LittleLua() : _state(NULL) {}
  void init();

  void close();

  void execute(const char * code, Callback _postOperation = NULL);
  void execute(const string code, Callback _postOperation = NULL);
  void executeFile(const char * path, Callback _postOperation = NULL);

  #pragma region getters

  int getInteger(const char * variable);
  double getNumber(const char * variable); // or getDouble
  bool getBool(const char * variable);
  string getString(const char * variable);

  vector<int> getIntegerVector(const char * variable);
  vector<double> getNumberVector(const char * variable);
  vector<bool> getBoolVector(const char * variable);
  vector<string> getStringVector(const char * variable);

  #pragma endregion getters

  ~LittleLua() {
    if (!_state)
      this->close();
  }

private:
#pragma region LittleLua_VARIABLES
  lua_State *_state;
  unsigned int idx = 0;
#pragma endregion LittleLua_VARIABLES
};
#endif // LittleLua_HPP