#include "LuaBindingsTest.h"
#include "Config.h"
#include "XULWin/Element.h"
#include "XULWin/WinUtils.h"
#include "XULWin/Lua/LuaBindings.h"
#include "XULWin/Lua/XULRunnerWithLua.h"
#include "Poco/Path.h"
#include <boost/bind.hpp>


namespace XULWin
{


    LuaBindingsTest::LuaBindingsTest()
    {    
        Poco::Path promptFile(__FILE__);
        std::string path = promptFile.parent().append("Logger.xul").toString();
        mLoggerApp = mLoggerRunner.loadXUL(path);
        if (Window * wnd = mLoggerApp->downcast<Window>())
        {
            wnd->impl()->move(0, 200, 400, 100);
            wnd->showNonModal(false);
        }
    }


    void LuaBindingsTest::runXULSample(const std::string & inAppname)
    {
        Windows::CurrentDirectoryChanger cd("../xulrunnersamples/" + inAppname + "/");

#if TEST_WITH_MOZILLA_XULRUNNER
        system("run.bat");
#endif

        Lua::XULRunnerWithLua xulRunner;
        xulRunner.Logger = boost::bind(Lua::showMessage, _1);
        ElementPtr rootEl = xulRunner.loadApplication("application.ini");

        if (Window * wnd = rootEl->downcast<Window>())
        {
            wnd->showModal();
        }
    }
    
    
    void LuaBindingsTest::log(const std::string & inMessage)
    {
        if (Element * logListBox = mLoggerApp->getElementById("logListBox"))
        {
            AttributesMapping attr;
            attr["label"] = inMessage;
            ListItem::Create(logListBox, attr)->init();
        }
    }


} // namespace XULWin
