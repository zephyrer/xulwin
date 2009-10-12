#include "LuaBindingsTest.h"
#include "LuaInitializer.h"
#include "XULWin/ErrorReporter.h"
#include <boost/bind.hpp>
#include <lua.hpp>
#include <stdio.h>
#include <windows.h> // TODO: remove?


namespace XULWin
{

    void DisplayError(const std::string & inMessage)
    {
        ::MessageBoxA(0, inMessage.c_str(), "XULWin", MB_OK);
    }


    LuaBindingsTest::LuaBindingsTest()
    {
        mInitializer.reset(new Lua::LuaInitializer);
        mInitializer->Logger = boost::bind(&DisplayError, _1);
    }


    LuaBindingsTest::~LuaBindingsTest()
    {
        mInitializer.reset();
    }


    void LuaBindingsTest::run()
    {
        mInitializer->loadScript("LuaBindings.ShowMessage(\"Hello From Lua\")");
        mInitializer->loadScript(
            "LuaBindings.Element el()       "
            "sum = el:testSum(1,2)          "
            "LuaBindings.ShowMessage(sum)   "
        );        
    }


} // namespace XULWin
