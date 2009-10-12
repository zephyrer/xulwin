#include "LuaInitializer.h"
#include <sstream>
#include <lua.hpp>


#ifdef __cplusplus
extern "C" {
#endif


extern int luaopen_LuaBindings(lua_State* L); // declare the wrapped module
#define LUA_EXTRALIBS {"LuaBindings",luaopen_LuaBindings}


#ifdef __cplusplus
}
#endif





namespace XULWin
{

namespace Lua
{


    lua_State * L;


    LuaInitializer::LuaInitializer()
    {    
        L = lua_open();
        luaopen_base(L);
        luaopen_LuaBindings(L);
    }


    LuaInitializer::~LuaInitializer()
    {
        lua_close(L);
    }


    void LuaInitializer::log(const std::string & inMessage)
    {
        if (Logger)
        {
            Logger(inMessage);
        }
    }

    
    void LuaInitializer::loadFile(const std::string & inLuaFile)
    {
        luaL_loadfile(L, inLuaFile.c_str());
        lua_pcall(L, 0, 0, 0);
    }

    
    bool LuaInitializer::loadScript(const std::string & inScript)
    {
        int status = luaL_loadstring(L, inScript.c_str());
        if (status != 0)
        {
            std::stringstream ss;
            ss << status;
            log(ss.str());
            return false;
        }

        status = lua_pcall(L, 0, 0, 0);
        if (status != 0)
        {
            log(lua_tostring(L, -1));
            return false;
        }
        return true;
    }


} // namespace Lua

} // namespace XULWin
