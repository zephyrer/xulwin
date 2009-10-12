#include "LuaBindingsTest.h"
#include "XULWin/Element.h"
#include "XULWin/WinUtils.h"
#include "XULWin/Lua/LuaBindings.h"
#include "XULWin/Lua/XULRunnerWithLua.h"
#include <boost/bind.hpp>


namespace XULWin
{


    void LuaBindingsTest::runXULSample(const std::string & inAppname)
    {   
        Windows::CurrentDirectoryChanger cd("../xulrunnersamples/" + inAppname + "/");
        Lua::XULRunnerWithLua xulRunner;
        xulRunner.Logger = boost::bind(Lua::showMessage, _1);
        ElementPtr rootEl = xulRunner.loadApplication("application.ini");
        rootEl->downcast<Window>()->showModal();
    }


} // namespace XULWin
