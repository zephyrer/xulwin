#include "LuaBindingsTest.h"
#include "XULWin/Element.h"
#include "XULWin/WinUtils.h"


namespace XULWin
{


    LuaBindingsTest::LuaBindingsTest()
    {
    }


    LuaBindingsTest::~LuaBindingsTest()
    {
    }



    void LuaBindingsTest::run()
    {   
        Windows::CurrentDirectoryChanger cd("../xulrunnersamples/hello/");
        ElementPtr rootEl = mXULRunnerWithLua.loadApplication("application.ini");
        rootEl->downcast<Window>()->showModal();
    }



} // namespace XULWin
