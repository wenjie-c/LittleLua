#include <LuaException.hpp>
#include<LittleLua.hpp>
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

#pragma region Macros

// #define DESERIALIZE_MEMBER(x) this->x = getInteger("x"); // or
// getNumber("x"); or getBool("x"); or getString("x"); // it's hard to say from
// https://dev.to/sgf4/foreach-macro-in-c-48ic and it is possible to expand it
// more
#define N_VA_ARGS_(_9, _8, _7, _6, _5, _4, _3, _2, _1, N, ...) N
#define N_VA_ARGS(...)                                                         \
  N_VA_ARGS_(__VA_ARGS__ __VA_OPT__(, ) 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)

#define LITTLELUA_PASTE_0(FN, ...)
#define LITTLELUA_PASTE_1(FN, E, ...) FN(E)
#define LITTLELUA_PASTE_2(FN, E, ...) FN(E) LITTLELUA_PASTE_1(FN, __VA_ARGS__)
#define LITTLELUA_PASTE_3(FN, E, ...) FN(E) LITTLELUA_PASTE_2(FN, __VA_ARGS__)
#define LITTLELUA_PASTE_4(FN, E, ...) FN(E) LITTLELUA_PASTE_3(FN, __VA_ARGS__)
#define LITTLELUA_PASTE_5(FN, E, ...) FN(E) LITTLELUA_PASTE_4(FN, __VA_ARGS__)
#define LITTLELUA_PASTE_6(FN, E, ...) FN(E) LITTLELUA_PASTE_5(FN, __VA_ARGS__)
#define LITTLELUA_PASTE_7(FN, E, ...) FN(E) LITTLELUA_PASTE_6(FN, __VA_ARGS__)
#define LITTLELUA_PASTE_8(FN, E, ...) FN(E) LITTLELUA_PASTE_7(FN, __VA_ARGS__)
#define LITTLELUA_PASTE_9(FN, E, ...) FN(E) LITTLELUA_PASTE_8(FN, __VA_ARGS__)
#define LITTLELUA_PASTE_10(FN, E, ...) FN(E) LITTLELUA_PASTE_9(FN, __VA_ARGS__)
#define LITTLELUA_PASTE_11(FN, E, ...) FN(E) LITTLELUA_PASTE_10(FN, __VA_ARGS__)
#define LITTLELUA_PASTE_12(FN, E, ...) FN(E) LITTLELUA_PASTE_11(FN, __VA_ARGS__)
#define LITTLELUA_PASTE_13(FN, E, ...) FN(E) LITTLELUA_PASTE_12(FN, __VA_ARGS__)
#define LITTLELUA_PASTE_14(FN, E, ...) FN(E) LITTLELUA_PASTE_13(FN, __VA_ARGS__)
#define LITTLELUA_PASTE_15(FN, E, ...) FN(E) LITTLELUA_PASTE_14(FN, __VA_ARGS__)

#define LITTLELUA_PASTE__(FN, NARGS, ...)                                      \
  LITTLELUA_PASTE_##NARGS(FN, __VA_ARGS__)
#define LITTLELUA_PASTE_(FN, NARGS, ...)                                       \
  LITTLELUA_PASTE__(FN, NARGS, __VA_ARGS__)

#define LITTLELUA_PASTE(FN, ...)                                               \
  LITTLELUA_PASTE_(FN, N_VA_ARGS(__VA_ARGS__), __VA_ARGS__)

#define DESERIALIZE_MEMBER(PAIR) DESERIALIZE_MEMBER_IMPL PAIR

/* #define DESERIALIZE_MEMBER_IMPL(y, x)  \
  lua_getfield(lua, idx,#x); \
  type = lua_type(lua, idx, -1);                                               \
  switch (type) {                                                              \
  case LUA_TNUMBER:                                                            \
    this->x = (double)(lua_tonumber(lua, -1)); \
    break;                                                                     \
  case LUA_TBOOLEAN:                                                           \
    this->x = (bool)(lua_toboolean(lua, -1)); \
    break;                                                                     \
  case LUA_TSTRING:                                                            \
    this->x = (y)lua_tostring(lua, -1); \
    break;                                                                     \
  default:                                                                     \
    throw LuaException("Unknown lua type on deserialization.");                \
  }                                                                            \
  --idx;   */

/*
#define DESERIALIZE_MEMBER_IMPL(y, x)                                          \
  lua_getfield(lua, idx, #x);                                                  \
  type = lua_type(lua, -1);                                               \
  if constexpr ((is_same<y, int>::value or is_same<y,int>::value or \
                 is_same<y, long>::value || is_same<y, float>::value || \
                 is_same<y, double>::value || is_same<y,double>::value ))
{if(type == \
                LUA_TNUMBER) {                                                 \
    this->x = static_cast<y>(lua_tonumber(lua, -1));                           \
  }} else if constexpr (is_same<y, bool>::value) {if (type == LUA_TBOOLEAN) { \
    this->x = static_cast<y>(lua_toboolean(lua, -1));                          \
  }} else if constexpr (is_same<y, string>::value) {if(type == LUA_TSTRING) { \
    this->x = string(lua_tostring(lua, -1));                                   \
  }} else { \
    throw LuaException("Unknown lua type on deserialization.");                \
  }                                                                            \
  --idx;

#define DESERIALIZE_FUNCTION(TYPE, ...)                                        \
  void from_lua(lua_State *lua) {                                              \
    int type = -1;                                                             \
    int idx = -2;                                                              \
    LITTLELUA_PASTE(DESERIALIZE_MEMBER, __VA_ARGS__)                           \
  }
*/

// These works
//#define LITTLELUA_DESERIALIZE_INIT() int idx = -1;

#pragma region DESERIALIZATION

#define LITTLELUA_DESERIALIZE_INT_MEMBER(x)                                    \
  lua_getfield(lua, -1, #x);                                                   \
  if (!lua_isinteger(lua, -1))                                                 \
    throw LuaException("This member is not an integer.");                      \
  this->x = lua_tointeger(lua, -1);                                            \
  lua_pop(lua, 1);
#define LITTLELUA_DESERIALIZE_DOUBLE_MEMBER(x)                                 \
  lua_getfield(lua, -1, #x);                                                   \
  if (!lua_isnumber(lua, -1))                                                  \
    throw LuaException("This member is not a double or float.");               \
  this->x = lua_tonumber(lua, -1);                                             \
  lua_pop(lua, 1);
#define LITTLELUA_DESERIALIZE_BOOL_MEMBER(x)                                   \
  lua_getfield(lua, -1, #x);                                                   \
  if (!lua_isboolean(lua, -1))                                                 \
    throw LuaException("This member is not a boolean.");                       \
  this->x = lua_toboolean(lua, -1);                                            \
  lua_pop(lua, 1);
#define LITTLELUA_DESERIALIZE_STRING_MEMBER(x)                                 \
  lua_getfield(lua, -1, #x);                                                   \
  if (!lua_isstring(lua, -1))                                                  \
    throw LuaException("This member is not a string.");                        \
  this->x = lua_tostring(lua, -1);                                             \
  lua_pop(lua, 1);

/**
 * @brief Deserialize constructor definition
 * @param lua Internal lua_State pointer.
 */
#define LITTLELUA_DESERIALIZE_CONSTRUCTOR(Type) Type(lua_State *lua)

/**
 * @brief Alternative to deserialization constructor using a method. You need an
 * empty constructor anyways.
 *
 */
#define LITTLELUA_DESERIALIZE_METHOD(Type) void fromLua(lua_State *lua)

#pragma endregion DESERIALIZATION

#pragma region SERIALIZATION

#define LITTLELUA_SERIALIZE_INTEGER_MEMBER(x)                                  \
  lua_pushinteger(lua, x);                                                     \
  lua_setfield(lua, -2, #x);

#define LITTLELUA_SERIALIZE_DOUBLE_MEMBER(x)                                   \
  lua_pushnumber(lua, x);                                                      \
  lua_setfield(lua, -2, #x);

#define LITTLELUA_SERIALIZE_BOOL_MEMBER(x)                                     \
  lua_pushboolean(lua, x);                                                     \
  lua_setfield(lua, -2, #x);

#define LITTLELUA_SERIALIZE_CSTRING_MEMBER(x)                                  \
  lua_pushstring(lua, x);                                                      \
  lua_setfield(lua, -2, #x);

#define LITTLELUA_SERIALIZE_STRING_MEMBER(x)                                   \
  lua_pushstring(lua, x.c_str());                                              \
  lua_setfield(lua, -2, #x);

#define LITTLELUA_SERIALIZE void toLua(lua_State *lua)

#pragma endregion SERIALIZATION

#pragma region FUNCTIONAL

/* #define LUA_FUNCTION(littleLua,function_name, , ...) [littleLua](...){\
  \
}
 */

/**
 * @brief Begin to call a lua void function. It is followed by the argument
 * declaration using LUA_ARGS_<type>() macros
 *
 */
#define LUA_FUNCTION_BEGIN(function_name) callFunction(#function_name, \
  [=](lua_State* lua){ \
    unsigned int counter = 0;

/**
 * @brief Begin to call a lua int function. It is followed by the argument
 * declaration using LUA_ARGS_<type>() macros
 *
 */
#define LUA_INT_FUNCTION_BEGIN(function_name) callIntegerFunction(#function_name, \
    [=](lua_State* lua){ \
      unsigned int counter = 0;

/**
 * @brief Begin to call a lua double function. It is followed by the argument
 * declaration using LUA_ARGS_<type>() macros
 *
 */
#define LUA_DOUBLE_FUNCTION_BEGIN(function_name) callNumberFunction(#function_name, \
    [=](lua_State* lua){ \
      unsigned int counter = 0;

/**
 * @brief Begin to call a lua bool function. It is followed by the argument
 * declaration using LUA_ARGS_<type>() macros
 *
 */
#define LUA_BOOL_FUNCTION_BEGIN(function_name) callBoolFunction(#function_name, \
  [=](lua_State* lua){ \
    unsigned int counter = 0;

/**
 * @brief Begin to call a lua string function. It is followed by the argument
 * declaration using LUA_ARGS_<type>() macros
 *
 */
#define LUA_STRING_FUNCTION_BEGIN(function_name) callStringFunction(#function_name, \
  [=](lua_State* lua){ \
    unsigned int counter = 0;

/**
 * @brief Passing an integer parameter
 *
 */
#define LUA_ARG_INT(x)                                                         \
  lua_pushinteger(lua,x);                                                          \
  counter++;

/**
 * @brief Passing an double parameter
 *
 */
#define LUA_ARG_DOUBLE(x)                                                      \
  lua_pushnumber(lua,x);                                                           \
  counter++;

/**
 * @brief Passing an bool parameter
 *
 */
#define LUA_ARG_BOOL(x)                                                        \
  lua_pushboolean(lua,x);                                                          \
  counter++;

/**
 * @brief Passing an string parameter
 *
 */
#define LUA_ARG_STRING(x)                                                      \
  lua_pushstring(lua,x);                                                           \
  counter++;

/**
 * @brief End the call of the lua function
 *
 */
#define LUA_FUNCTION_END                                                       \
  return counter;                                                              \
  });

#define LUA_FUNCTION_RETURN_NUMBER 1
#pragma endregion FUNCTIONAL

#pragma endregion Macros