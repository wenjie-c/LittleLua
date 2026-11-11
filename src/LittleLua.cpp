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
    auto value = lua_tointeger(_state, -1);
    lua_pop(_state, 1);
    return value;
  } else
    throw LuaException("The top stack variable is not an integer.");
}
double LittleLua::getNumber(const char *variable) {
  lua_getglobal(_state, variable);
  if (lua_isnumber(_state, -1)) {
    auto value = lua_tonumber(_state, -1);
    lua_pop(_state, 1);
    return value;
  } else
    throw LuaException("The top stack variable is not a double.");
}
bool LittleLua::getBool(const char *variable) {
  lua_getglobal(_state, variable);
  if (lua_isboolean(_state, -1)) {
    auto value = lua_toboolean(_state, -1);
    lua_pop(_state, 1);
    return value;
  } else
    throw LuaException("The top stack variable is not a boolean.");
}
string LittleLua::getString(const char *variable) {
  lua_getglobal(_state, variable);
  if (lua_isstring(_state, -1)) {
    auto value = lua_tostring(_state, -1);
    lua_pop(_state, 1);
    return value;
  } else
    throw LuaException("The top stack variable is not a string.");
}

vector<int> LittleLua::getIntegerVector(const char *variable) {
  // idx = 1; // tracking array front
  vector<int> res;
  lua_getglobal(_state, variable);
  if (lua_istable(_state, -1)) {
    for (int idx = 1; lua_geti(_state, -1, idx); idx++) {
      // Always push the first value to top
      if (lua_isinteger(_state, -1)) {
        res.push_back(lua_tointeger(_state, -1));
        lua_pop(_state, 1);
      } else
        throw LuaException("The top stack variable is not an integer.");
    }
  } else
    // throw LuaException("The top stack variable is not an array or a table.");
    throw LuaException(lua_tostring(_state, -1));
  lua_pop(_state, 2);
  return res;
}

vector<double> LittleLua::getNumberVector(const char *variable) {
  // idx = 1; // tracking array front
  vector<double> res;
  lua_getglobal(_state, variable);
  if (lua_istable(_state, -1)) {
    for (int idx = 1; lua_geti(_state, -1, idx); idx++) {
      // Always push the first value to top
      if (lua_isnumber(_state, -1)) {
        res.push_back(lua_tonumber(_state, -1));
        lua_pop(_state, 1);
      } else
        throw LuaException("The top stack variable is not a double.");
    }
  } else
    // throw LuaException("The top stack variable is not an array or a table.");
    throw LuaException(lua_tostring(_state, -1));
    lua_pop(_state, 2);
  return res;
}
vector<bool> LittleLua::getBoolVector(const char *variable) {
  // idx = 1; // tracking array front
  vector<bool> res;
  lua_getglobal(_state, variable);
  if (lua_istable(_state, -1)) {
    for (int idx = 1; lua_geti(_state, -1, idx); idx++) {
      // Always push the first value to top
      if (lua_isboolean(_state, -1)) {
        res.push_back(lua_toboolean(_state, -1));
        lua_pop(_state, 1);
      } else
        throw LuaException("The top stack variable is not an integer.");
    }
  } else
    // throw LuaException("The top stack variable is not an array or a table.");
    throw LuaException(lua_tostring(_state, -1));
    lua_pop(_state, 2);
  return res;
}
vector<string> LittleLua::getStringVector(const char *variable) {
  // idx = 1; // tracking array front
  vector<string> res;
  lua_getglobal(_state, variable);
  if (lua_istable(_state, -1)) {
    for (int idx = 1; lua_geti(_state, -1, idx); idx++) {
      // Always push the first value to top
      if (lua_isstring(_state, -1)) {
        res.push_back(lua_tostring(_state, -1));
        lua_pop(_state, 1);
      } else
        throw LuaException("The top stack variable is not a string.");
    }
  } else
    // throw LuaException("The top stack variable is not an array or a table.");
    throw LuaException(lua_tostring(_state, -1));
    lua_pop(_state, 2);
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
  lua_getfield(_state, -1, variable);
  if (lua_isinteger(_state, -1)) {
    //_idx--; // tracking the table
    auto value = lua_tointeger(_state, -1);
    lua_pop(_state, 1);
    return value;
  } else
    throw LuaException(lua_tostring(_state, -1));
    lua_pop(_state, 1);
  return 0;
}

double LittleLua::getNumberFromTable(const char *variable) {
  lua_getfield(_state, -1, variable);
  if (lua_isnumber(_state, -1)) {
    //_idx--; // tracking the table
    auto value = lua_tonumber(_state, -1);
    lua_pop(_state, 1);
    return value;
  } else
    throw LuaException(lua_tostring(_state, -1));
    lua_pop(_state, 1);
  return 0;
}

bool LittleLua::getBoolFromTable(const char *variable) {
  lua_getfield(_state, -1, variable);
  if (lua_isboolean(_state, -1)) {
    //_idx--; // tracking the table
    auto value = lua_toboolean(_state, -1);
    lua_pop(_state, 1);
    return value;
  } else
    throw LuaException(lua_tostring(_state, -1));
    lua_pop(_state, 1);
  return false;
}
string LittleLua::getStringFromTable(const char *variable) {
  lua_getfield(_state, -1, variable);
  if (lua_isstring(_state, -1)) {
    //_idx--; // tracking the table
    auto value = lua_tostring(_state, -1);
    lua_pop(_state, 1);
    return value;
  } else
    LuaException(lua_tostring(_state, -1));
    lua_pop(_state, 1);
  return string("");
}

template <typename T> T LittleLua::deserializate(const char *variable) {
  // Deserialization using the member function
  /* T container;
  this->openTable(variable);
  container.from_lua(_state, _idx);
  return container; */
  // Deserialization using the constructor
  this->openTable(variable);
  return T(_state);
}

vector<int> LittleLua::getIntegerVectorFromTable(const char *variable) {
  vector<int> res;
  lua_getfield(_state, -1, variable);
  if (lua_istable(_state, -1)) {
    //_idx--;
    for (int idx = 1; lua_geti(_state, -1, idx); idx++) {
      // Always push the first value to top
      if (lua_isinteger(_state, -1)) {
        //_idx--;
        res.push_back(lua_tointeger(_state, -1));
        lua_pop(_state, 1);
      } else
        throw LuaException("The top stack variable is not an integer.");
    }
  } else
    throw LuaException(lua_tostring(_state, -1));
  lua_pop(_state,
          2); // [in https://www.lua.org/manual/5.4/manual.html , section 2.1 –
              // Values and Types] explains that when you try to get a non
              // existent key, pushes a nil onto the top of the stack
  return res;
}
vector<double> LittleLua::getNumberVectorFromTable(const char *variable) {
  vector<double> res;
  lua_getfield(_state, -1, variable);
  if (lua_istable(_state, -1)) {
    //_idx--;
    for (int idx = 1; lua_geti(_state, -1, idx); idx++) {
      // Always push the first value to top
      if (lua_isnumber(_state, -1)) {
        //_idx--;
        res.push_back(lua_tonumber(_state, -1));
        lua_pop(_state, 1);
      } else
        throw LuaException("The top stack variable is not a number.");
    }
  } else
    throw LuaException(lua_tostring(_state, -1));
  lua_pop(_state, 2);
  return res;
}
vector<bool> LittleLua::getBoolVectorFromTable(const char *variable) {
  vector<bool> res;
  lua_getfield(_state, -1, variable);
  if (lua_istable(_state, -1)) {
    //_idx--;
    for (int idx = 1; lua_geti(_state, -1, idx); idx++) {
      // Always push the first value to top
      if (lua_isboolean(_state, -1)) {
        //_idx--;
        res.push_back(lua_toboolean(_state, -1));
        lua_pop(_state, 1);
      } else
        throw LuaException("The top stack variable is not an integer.");
    }
  } else
    throw LuaException(lua_tostring(_state, -1));
  lua_pop(_state, 2);
  return res;
}
vector<string> LittleLua::getStringVectorFromTable(const char *variable) {
  vector<string> res;
  lua_getfield(_state, -1, variable);
  if (lua_istable(_state, -1)) {
    //_idx--;
    for (int idx = 1; lua_geti(_state, -1, idx); idx++) {
      // Always push the first value to top
      if (lua_isstring(_state, -1)) {
        //_idx--;
        res.push_back(lua_tostring(_state, -1));
        lua_pop(_state, 1);
      } else
        throw LuaException("The top stack variable is not an integer.");
    }
  } else
    throw LuaException(lua_tostring(_state, -1));
  lua_pop(_state, 2);
  return res;
}

#pragma endregion getters

#pragma region setters
void LittleLua::setInteger(const char *name, const int value) {
  lua_pushinteger(_state, value);
  lua_setglobal(_state, name);
}
void LittleLua::setNumber(const char *name, const double value) {
  lua_pushnumber(_state, value);
  lua_setglobal(_state, name);
}
void LittleLua::setBool(const char *name, const bool value) {
  lua_pushboolean(_state, value);
  lua_setglobal(_state, name);
}
void LittleLua::setString(const char *name, const char *value) {
  lua_pushstring(_state, value);
  lua_setglobal(_state, name);
}
void LittleLua::setString(const char *name, const string value) {
  setString(name, value.c_str());
}

void LittleLua::setIntegerFromTable(const char *name, const int value) {
  lua_pushinteger(_state, value);
  //--_idx;
  lua_setfield(_state, -2,
               name); // idx is fixed on -2 because everytime that a value is
                      // setted, the value is popped out by lua
}
void LittleLua::setNumberFromTable(const char *name, const double value) {
  lua_pushnumber(_state, value);
  //--_idx;
  lua_setfield(_state, -2, name);
}
void LittleLua::setBoolFromTable(const char *name, const bool value) {
  lua_pushboolean(_state, value);
  //--_idx;
  lua_setfield(_state, -2, name);
}
void LittleLua::setStringFromTable(const char *name, const char *value) {
  lua_pushstring(_state, value);
  //--_idx;
  lua_setfield(_state, -2, name);
}

void LittleLua::setStringFromTable(const char *name, const string value) {
  setStringFromTable(name, value.c_str());
}

void LittleLua::setIntegerVector(const char *name, const vector<int> integers) {
  lua_newtable(_state);
  for (size_t i = 0; i < integers.size(); i++) {
    lua_pushinteger(_state, i + 1); // index on lua starts in 1
    lua_pushinteger(_state, integers[i]);
    lua_settable(_state, -3);
  }

  lua_setglobal(_state, name);
}
void LittleLua::setNumberVector(const char *name,
                                const vector<double> doubles) {
  lua_newtable(_state);
  for (size_t i = 0; i < doubles.size(); i++) {
    lua_pushnumber(_state, i + 1); // index on lua starts in 1
    lua_pushnumber(_state, doubles[i]);
    lua_settable(_state, -3);
  }

  lua_setglobal(_state, name);
}
void LittleLua::setBoolVector(const char *name, const vector<bool> bools) {
  lua_newtable(_state);
  for (size_t i = 0; i < bools.size(); i++) {
    lua_pushboolean(_state, i + 1); // index on lua starts in 1
    lua_pushboolean(_state, bools[i]);
    lua_settable(_state, -3);
  }

  lua_setglobal(_state, name);
}
void LittleLua::setStringVector(const char *name,
                                const vector<string> strings) {
  lua_newtable(_state);
  for (size_t i = 0; i < strings.size(); i++) {
    lua_pushinteger(_state, i + 1); // index on lua starts in 1
    lua_pushstring(_state, strings[i].c_str());
    lua_settable(_state, -3);
  }

  lua_setglobal(_state, name);
}

#pragma endregion setters
