#include <LittleLua.hpp>

void LittleLua::init() {
  _state = luaL_newstate();
  if (!_state)
    throw LuaException("Failed to create lua state");
  luaL_openlibs(_state);
}

void LittleLua::close() { lua_close(_state); }

void LittleLua::execute(const char *code, Callback _postOperation) {
  if (luaL_dostring(_state, code) == LUA_OK) {
    if (_postOperation)
      _postOperation(_state);
  } else {
    // luaL_error(_state, "Error: %s\n", lua_tostring(_state, -1));
    throw LuaException(lua_tostring(_state, -1));
  }
}
void LittleLua::execute(const string code, Callback _postOperation) {
  this->execute(code.c_str(), _postOperation);
}
void LittleLua::executeFile(const char *path, Callback _postOperation) {
  if (luaL_dofile(_state, path) == LUA_OK) {
    if (_postOperation)
      _postOperation(_state);
  } else {
    // luaL_error(_state, "Error: %s\n", lua_tostring(_state, -1));
    throw LuaException(lua_tostring(_state, -1));
  }
}

#pragma region getters

int LittleLua::getInteger(const char *variable) {
  lua_getglobal(_state, variable);
  if (lua_isinteger(_state, -1)) {
    return lua_tointeger(_state, -1);
  } else
    throw LuaException("The top stack variable is not an integer.");
}
double LittleLua::getNumber(const char *variable) {
  lua_getglobal(_state, variable);
  if (lua_isnumber(_state, -1)) {
    return lua_tonumber(_state, -1);
  } else
    throw LuaException("The top stack variable is not a double.");
}
bool LittleLua::getBool(const char *variable) {
  lua_getglobal(_state, variable);
  if (lua_isboolean(_state, -1)) {
    return lua_toboolean(_state, -1);
  } else
    throw LuaException("The top stack variable is not a boolean.");
}
string LittleLua::getString(const char *variable) {
  lua_getglobal(_state, variable);
  if (lua_isstring(_state, -1)) {
    return lua_tostring(_state, -1);
  } else
    throw LuaException("The top stack variable is not a string.");
}

vector<int> LittleLua::getIntegerVector(const char *variable) {
  // idx = 1; // tracking array front
  vector<int> res;
  lua_getglobal(_state, variable);
  if (lua_istable(_state, -1)) {
    for (int idx = -1; lua_geti(_state, idx, -idx); idx--) {
      // Always push the first value to top
      if (lua_isnumber(_state, -1)) {
        res.push_back(lua_tointeger(_state, -1));
      } else
        throw LuaException("The top stack variable is not an integer.");
    }
  } else
    // throw LuaException("The top stack variable is not an array or a table.");
    throw LuaException(lua_tostring(_state, -1));
  return res;
}

vector<double> LittleLua::getNumberVector(const char *variable) {
  // idx = 1; // tracking array front
  vector<double> res;
  lua_getglobal(_state, variable);
  if (lua_istable(_state, -1)) {
    for (int idx = -1; lua_geti(_state, idx, -idx); idx--) {
      // Always push the first value to top
      if (lua_isnumber(_state, -1)) {
        res.push_back(lua_tonumber(_state, -1));
      } else
        throw LuaException("The top stack variable is not a double.");
    }
  } else
    // throw LuaException("The top stack variable is not an array or a table.");
    throw LuaException(lua_tostring(_state, -1));
  return res;
}
vector<bool> LittleLua::getBoolVector(const char *variable) {
  // idx = 1; // tracking array front
  vector<bool> res;
  lua_getglobal(_state, variable);
  if (lua_istable(_state, -1)) {
    for (int idx = -1; lua_geti(_state, idx, -idx); idx--) {
      // Always push the first value to top
      if (lua_isboolean(_state, -1)) {
        res.push_back(lua_toboolean(_state, -1));
      } else
        throw LuaException("The top stack variable is not an integer.");
    }
  } else
    // throw LuaException("The top stack variable is not an array or a table.");
    throw LuaException(lua_tostring(_state, -1));
  return res;
}
vector<string> LittleLua::getStringVector(const char *variable) {
  // idx = 1; // tracking array front
  vector<string> res;
  lua_getglobal(_state, variable);
  if (lua_istable(_state, -1)) {
    for (int idx = -1; lua_geti(_state, idx, -idx); idx--) {
      // Always push the first value to top
      if (lua_isstring(_state, -1)) {
        res.push_back(lua_tostring(_state, -1));
      } else
        throw LuaException("The top stack variable is not a string.");
    }
  } else
    // throw LuaException("The top stack variable is not an array or a table.");
    throw LuaException(lua_tostring(_state, -1));
  return res;
}

void LittleLua::openTable(const char *variable) {
  _idx = -1;
  lua_getglobal(_state, variable);
  if (lua_istable(_state, _idx)) {
    //--_idx; // tracking the table
  } else
    throw LuaException("Couldn't open the table, it is not a table.");
}
int LittleLua::getIntegerFromTable(const char *variable) {
  lua_getfield(_state, _idx, variable);
  if (lua_isinteger(_state, -1)) {
    _idx--; // tracking the table
    return lua_tointeger(_state, -1);
  } else
    LuaException(lua_tostring(_state, -1));
  return 0;
}

double LittleLua::getNumberFromTable(const char *variable) {
  lua_getfield(_state, _idx, variable);
  if (lua_isnumber(_state, -1)) {
    _idx--; // tracking the table
    return lua_tonumber(_state, -1);
  } else
    LuaException(lua_tostring(_state, -1));
  return 0;
}

bool LittleLua::getBoolFromTable(const char *variable) {
  lua_getfield(_state, _idx, variable);
  if (lua_isboolean(_state, -1)) {
    _idx--; // tracking the table
    return lua_toboolean(_state, -1);
  } else
    LuaException(lua_tostring(_state, -1));
  return false;
}
string LittleLua::getStringFromTable(const char *variable) {
  lua_getfield(_state, _idx, variable);
  if (lua_isstring(_state, -1)) {
    _idx--; // tracking the table
    return lua_tostring(_state, -1);
  } else
    LuaException(lua_tostring(_state, -1));
  return string("");
}

#pragma endregion getters
