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


    void LuaBindingsTest::run(const std::string & inAppname)
    {   
        Windows::CurrentDirectoryChanger cd("../xulrunnersamples/" + inAppname + "/");
        ElementPtr rootEl = mXULRunnerWithLua.loadApplication("application.ini");
        rootEl->downcast<Window>()->showModal();
    }



} // namespace XULWin
