#include <LuaException.hpp>
#include <cstddef>
#include <iostream>
#include <string>
#include <type_traits>
#include <vector>

extern "C" {
#include <lauxlib.h>
#include <lua.h>
#include <lualib.h>
}

#ifndef LittleLua_HPP
#define LittleLua_HPP

using namespace std;

typedef void (*Callback)(lua_State *);

class LittleLua {
public:
  LittleLua() : _state(NULL) {}
  void init();

  void close();

  void execute(const char *code, Callback _postOperation = NULL);
  void execute(const string code, Callback _postOperation = NULL);
  void executeFile(const char *path, Callback _postOperation = NULL);

#pragma region getters

  int getInteger(const char *variable);
  double getNumber(const char *variable); // or getDouble
  bool getBool(const char *variable);
  string getString(const char *variable);

  vector<int> getIntegerVector(const char *variable);
  vector<double> getNumberVector(const char *variable);
  vector<bool> getBoolVector(const char *variable);
  vector<string> getStringVector(const char *variable);

  /**
   * @brief Get into a lua table, the internal _idx is going to track it. It
   * only reaches 1 depth level.
   *
   * @param variable The name of the table
   */
  void openTable(const char *variable);
  int getIntegerFromTable(const char *variable);
  double getNumberFromTable(const char *variable);
  bool getBoolFromTable(const char *variable);
  string getStringFromTable(const char *variable);

  // TODO finish these getters
  vector<int> getIntegerVectorFromTable(const char *variable);
  vector<double> getNumberVectorFromTable(const char *variable);
  vector<bool> getBoolVectorFromTable(const char *variable);
  vector<string> getStringVectorFromTable(const char *variable);

#pragma endregion getters

// Use it before executing a file or code
#pragma region setters
  void setInteger(const char *name, const int value);
  void setNumber(const char *name, const double value);
  void setBool(const char *name, const bool value);
  void setString(const char *name, const char *value);
  void setString(const char *name, const string value);

  void setIntegerFromTable(const char *name, const int value);
  void setNumberFromTable(const char *name, const double value);
  void setBoolFromTable(const char *name, const bool value);
  void setStringFromTable(const char *name, const char *value);
#pragma endregion setters

  template <typename T> T deserializate(const char *variable);

  ~LittleLua() {
    if (!_state)
      this->close();
  }

  void dumpstack() {
    int top = lua_gettop(_state);
    for (int i = 1; i <= top; i++) {
      printf("%d\t%s\t", i, luaL_typename(_state, i));
      switch (lua_type(_state, i)) {
      case LUA_TNUMBER:
        printf("%g\n", lua_tonumber(_state, i));
        break;
      case LUA_TSTRING:
        printf("%s\n", lua_tostring(_state, i));
        break;
      case LUA_TBOOLEAN:
        printf("%s\n", (lua_toboolean(_state, i) ? "true" : "false"));
        break;
      case LUA_TNIL:
        printf("%s\n", "nil");
        break;
      default:
        printf("%p\n", lua_topointer(_state, i));
        break;
      }
    }
  }

private:
#pragma region LittleLua_VARIABLES
  lua_State *_state;
  int _idx = -1;
#pragma endregion LittleLua_VARIABLES
};
#endif // LittleLua_HPP