#include <cstdio>
#include <string>
#include <cstring>
#include <LuaSerializable.hpp>
#include <macros.hpp>
using namespace std;

/**
 * @brief An example class to show how serialization/deserialization works.
 * 
 */
class Settings : public ILuaSerializable {
    public:
    Settings() {}
    Settings(int w, int h, bool d, string t) :width(w), height(h), debug(d), title(t) {}
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

    LITTLELUA_SERIALIZE {
        LITTLELUA_SERIALIZE_INTEGER_MEMBER(width)
        LITTLELUA_SERIALIZE_INTEGER_MEMBER(height)
        LITTLELUA_SERIALIZE_BOOL_MEMBER(debug)
        LITTLELUA_SERIALIZE_STRING_MEMBER(title)
    }
};