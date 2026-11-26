#include <LuaException.hpp>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <iostream>
#include <string>
#include <tuple>
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

  /**
   * @brief Step into a lua table, the internal _idx used to track it.
   *
   * @param variable The name of the table
   */
  void openTable(const char *variable);

  /**
   * @brief Step out from a lua table.
   *
   */
  void closeTable();

#pragma region getters

  int getInteger(const char *variable);
  double getNumber(const char *variable); // or getDouble
  bool getBool(const char *variable);
  string getString(const char *variable);

  vector<int> getIntegerVector(const char *variable);
  vector<double> getNumberVector(const char *variable);
  vector<bool> getBoolVector(const char *variable);
  vector<string> getStringVector(const char *variable);

  int getIntegerFromTable(const char *member);
  double getNumberFromTable(const char *member);
  bool getBoolFromTable(const char *member);
  string getStringFromTable(const char *member);

  vector<int> getIntegerVectorFromTable(const char *member);
  vector<double> getNumberVectorFromTable(const char *member);
  vector<bool> getBoolVectorFromTable(const char *member);
  vector<string> getStringVectorFromTable(const char *member);

#pragma endregion getters

// Use it before executing a file or code
#pragma region setters
  void setInteger(const char *variable, const int value);
  void setNumber(const char *variable, const double value);
  void setBool(const char *variable, const bool value);
  void setString(const char *variable, const char *value);
  void setString(const char *variable, const string value);

  void setIntegerVector(const char *variable, const vector<int> integers);
  void setNumberVector(const char *variable, const vector<double> doubles);
  void setBoolVector(const char *variable, const vector<bool> bools);
  void setStringVector(const char *variable, const vector<string> strings);

  void setIntegerFromTable(const char *member, const int value);
  void setNumberFromTable(const char *member, const double value);
  void setBoolFromTable(const char *member, const bool value);
  void setStringFromTable(const char *member, const char *value);
  void setStringFromTable(const char *member, const string value);

  void setIntegerVectorFromTable(const char *member, const vector<int> value);
  void setNumberVectorFromTable(const char *member, const vector<double> value);
  void setBoolVectorFromTable(const char *member, const vector<bool> value);
  void setStringVectorFromTable(const char *member, const vector<string> value);

  /**
   * @brief Create a Table object from scratch
   *
   * @param variable
   */
  void createTable(const char *variable);

#pragma endregion setters

  /**
   * @brief Deserialize an object with LITTLELUA_SERIALIZE_CONSTRUCTOR macro
   * implemented
   *
   * @tparam T
   * @param variable Input Table name in Lua
   * @return T
   */
  template <typename T> T deserializate(const char *variable);

  /**
   * @brief Serialize an object with LITTLELUA_SERIALIZE_METHOD macro
   * implemented
   *
   * @param variable Output Table name to Lua
   */
  template <typename T> void serialize(const char *variable, T &object);

#pragma region functional

  /**
   * @brief Call a lua void function with variable arguments.
   *
   * @tparam argTypes
   * @param functionName
   * @param argList A function pointer to set all the arguments in order, it
   * returns the number of arguments. Use the macro LUA_ARGS(your_variable).
   */
  void callFunction(const char *functionName, function<size_t(lua_State* lua)> argList);

  /**
   * @brief Call a lua integer function with variable arguments.
   *
   * @param functionName
   * @param argList A function pointer to set all the arguments in order, it
   * returns the number of arguments. Use the macro LUA_ARGS(your_variable).
   */
  int callIntegerFunction(const char *functionName,
                          function<size_t(lua_State *)> argList);

  /**
   * @brief Call a lua double function with variable arguments.
   *
   * @param functionName
   * @param argList A function pointer to set all the arguments in order, it
   * returns the number of arguments. Use the macro LUA_ARGS(your_variable).
   */
  double callNumberFunction(const char *functionName,
                            function<size_t(lua_State *)> argList);

  /**
   * @brief Call a lua bool function with variable arguments.
   *
   * @param functionName
   * @param argList A function pointer to set all the arguments in order, it
   * returns the number of arguments. Use the macro LUA_ARGS(your_variable).
   */
  bool callBoolFunction(const char *functionName,
                        function<size_t(lua_State *)> argList);

  /**
   * @brief Call a lua string function with variable arguments.
   *
   * @param functionName
   * @param argList A function pointer to set all the arguments in order, it
   * returns the number of arguments. Use the macro LUA_ARGS(your_variable).
   */
  string callStringFunction(const char *functionName,
                            function<size_t(lua_State *)> argList);

#pragma endregion functional

  ~LittleLua() {
    if (!_state)
      this->close();
  }

  void dumpstack() {
    int top = lua_gettop(_state);
    printf("--- Dumping lua stack ---\n");
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

  /**
   * @brief Identifies and throws exception if an error status happens
   *
   * @param status
   */
  void _functionStatusHandler(int status);

#pragma endregion LittleLua_VARIABLES
};
#endif // LittleLua_HPP