#include <iostream>
extern "C" {
#include <lauxlib.h>
#include <lua.h>
#include <lualib.h>
}

#ifndef LuaSerializable_HPP
#define LuaSerializable_HPP

class ILuaSerializable {
public:
  ILuaSerializable() = default;
  virtual ~ILuaSerializable() = default;

  virtual void from_lua(lua_State * lua, int idx = -1) {}
};
#endif // LuaSerializable_HPP