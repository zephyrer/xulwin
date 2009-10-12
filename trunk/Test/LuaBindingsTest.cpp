#include "LuaBindingsTest.h"
#include "LuaInitializer.h"
#include "XULWin/ErrorReporter.h"
#include "XULWin/WinUtils.h"
#include "XULWin/XULRunner.h"
#include "XULWin/Lua/LuaBindings.h"
#include <boost/bind.hpp>
#include <lua.hpp>
#include <stdio.h>
#include <windows.h> // TODO: remove?


namespace XULWin
{

    void DisplayError(const std::string & inMessage)
    {
        ::MessageBoxA(0, ("Error: " + inMessage).c_str(), "XULWin", MB_OK);
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
        mInitializer->loadScript("XULWin.initialize()");
        mInitializer->loadScript("XULWin.showMessage(\"Hello From Lua\")");
        mInitializer->loadScript("el = XULWin.loadApplication(\"application.ini\")"
                                 "XULWin.showMessage(el:type())");
        mInitializer->loadScript("XULWin.showModal(el)");

        mInitializer->loadScript("XULWin.finalize()");
    }


} // namespace XULWin
