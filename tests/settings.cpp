#include <cstdio>
#include <string>
#include <cstring>
#include <LuaSerializable.hpp>
#include <macros.hpp>
using namespace std;


class Settings : public ILuaSerializable {
    public:
    Settings() {}
    LITTLELUA_DESERIALIZE_CONSTRUCTOR(Settings) {
        //LITTLELUA_DESERIALIZE_INIT()
        LITTLELUA_DESERIALIZE_INT_MEMBER(width)
        LITTLELUA_DESERIALIZE_INT_MEMBER(height)
        LITTLELUA_DESERIALIZE_BOOL_MEMBER(debug)
        LITTLELUA_DESERIALIZE_STRING_MEMBER(title)
    }
    unsigned int width = 0, height = 0;
    bool debug = false;
    string title;

    Settings(const Settings&) = delete; // forbid copy constructor

    string toString() {
        char buffer[512] = {0};
        sprintf(buffer, "Settings is:\n- width: %i\n- height: %i\n- debug: %d\n- title: %s.", width, height, debug, title.c_str());
        return string(buffer);
    }

    void from_lua(lua_State * lua, int idx = -1) override {
        LITTLELUA_DESERIALIZE_INT_MEMBER(width)
        LITTLELUA_DESERIALIZE_INT_MEMBER(height)
        LITTLELUA_DESERIALIZE_BOOL_MEMBER(debug)
        LITTLELUA_DESERIALIZE_STRING_MEMBER(title)
    }
};