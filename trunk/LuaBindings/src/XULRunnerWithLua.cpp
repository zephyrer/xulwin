#include "XULWin/Lua/XULRunnerWithLua.h"
#include "XULWin/Lua/LuaBindings.h"
#include "XULWin/ErrorReporter.h"
#include "XULWin/Initializer.h"
#include "XULWin/Script.h"
#include "XULWin/WinUtils.h"
#include "XULWin/XULRunner.h"
#include <sstream>
#include <lua.hpp>
#include <map>
#include <windows.h> // TODO: remove


#ifdef __cplusplus
extern "C" {
#endif

extern int luaopen_XULWin(lua_State* L); // declare the wrapped module
#define LUA_EXTRALIBS {"XULWin",luaopen_LuaBindings}

#ifdef __cplusplus
}
#endif



namespace XULWin
{


namespace Lua
{
    
    XULRunnerWithLua::XULRunnerWithLua() :
        mXULRunner(new XULRunner),
        mLuaState(0),
        mPrevRoot(0)
    {    
        
        mLuaState = lua_open();
        luaopen_base(mLuaState);
        luaopen_table(mLuaState);
        luaopen_string(mLuaState);
        luaopen_math(mLuaState);
        luaopen_XULWin(mLuaState);
    }


    XULRunnerWithLua::~XULRunnerWithLua()
    {
        Lua::setRootElement(mPrevRoot);
        lua_close(mLuaState);
    }


    void XULRunnerWithLua::log(const std::string & inMessage)
    {
        if (Logger)
        {
            Logger(inMessage);
        }
    }

    
    bool XULRunnerWithLua::loadFile(const std::string & inLuaFile)
    {
        int status = luaL_loadfile(mLuaState, inLuaFile.c_str());
        if (status != 0)
        {
            log(lua_tostring(mLuaState, -1));
            return false;
        }

        status = lua_pcall(mLuaState, 0, 0, 0);
        if (status != 0)
        {
            log(lua_tostring(mLuaState, -1));
            return false;
        }
        return true;
    }

    
    bool XULRunnerWithLua::loadScript(const std::string & inScript)
    {
        int status = luaL_loadstring(mLuaState, inScript.c_str());
        if (status != 0)
        {
            log(lua_tostring(mLuaState, -1));
            return false;
        }

        status = lua_pcall(mLuaState, 0, 0, 0);
        if (status != 0)
        {
            log(lua_tostring(mLuaState, -1));
            return false;
        }
        return true;
    }


    ElementPtr XULRunnerWithLua::loadApplication(const std::string & inApplicationIniFile)
    {
        ElementPtr result = mXULRunner->loadApplication(inApplicationIniFile);
        loadScripts(result.get());
        addListeners(result.get());
        mPrevRoot = Lua::setRootElement(result.get());
        return result;
    }


    ElementPtr XULRunnerWithLua::loadXUL(const std::string & inXULUrl)
    {
        ElementPtr result = mXULRunner->loadXUL(inXULUrl);
        loadScripts(result.get());
        addListeners(result.get());
        mPrevRoot = Lua::setRootElement(result.get());
        return result;
    }


    void XULRunnerWithLua::loadScripts(Element * inElement)
    {
        std::vector<XULWin::Script*> scripts;
        inElement->getElementsByType<Script>(scripts);
        for (size_t idx = 0; idx != scripts.size(); ++idx)
        {
            loadScript(scripts[idx]->innerText());
        }
    }


    void XULRunnerWithLua::addListeners(Element * inElement)
    {
        if (NativeComponent * comp = inElement->impl()->downcast<NativeComponent>())
        {
            comp->addEventListener(this);
        }
        for (size_t idx = 0; idx != inElement->children().size(); ++idx)
        {
            addListeners(inElement->children()[idx].get());
        }
    }


    LRESULT XULRunnerWithLua::handleCommand(Element * inSender, WORD inNotificationCode)
    {
        std::string oncommand = inSender->getAttribute("oncommand");
        if (!oncommand.empty())
        {
            loadScript(oncommand);
            return 0;
        }
        return 1;
    }


    LRESULT XULRunnerWithLua::handleMenuCommand(Element * inSender, WORD inMenuId)
    {
        return 1;
    }


    LRESULT XULRunnerWithLua::handleDialogCommand(Element * inSender, WORD inNotificationCode, WPARAM wParam, LPARAM lParam)
    {
        return 1;
    }


    LRESULT XULRunnerWithLua::handleMessage(Element * inSender, UINT inMessage, WPARAM wParam, LPARAM lParam)
    {
        return 1;
    }


} // namespace Lua

} // namespace XULWin
