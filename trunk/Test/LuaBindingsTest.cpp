#include "LuaBindingsTest.h"
#include "Config.h"
#include "XULWin/Element.h"
#include "XULWin/ErrorReporter.h"
#include "XULWin/ListItem.h"
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
            wnd->component()->move(0, 500, 400, 400);
            wnd->show(Window::DefaultPosition);
        }
        ErrorReporter::Instance().setLogger(boost::bind(&LuaBindingsTest::log, this, _1));
        ErrorCatcher errorCatcher;
        ReportError("Test logger");
    }


    void LuaBindingsTest::runXULSample(const std::string & inAppname)
    {
        Windows::CurrentDirectoryChanger cd("../xulrunnersamples/" + inAppname + "/");

#if TEST_WITH_MOZILLA_XULRUNNER
        ::ShellExecute(NULL, TEXT("open"), TEXT("run.bat"), NULL, NULL, SW_SHOWNORMAL);
#endif

        Lua::XULRunnerWithLua xulRunner;
        xulRunner.Logger = boost::bind(Lua::showMessage, _1);
        ElementPtr rootEl = xulRunner.loadApplication("application.ini");

        if (Window * wnd = rootEl->downcast<Window>())
        {
            wnd->showModal(Window::CenterInScreen);
        }
    }
    
    
    void LuaBindingsTest::log(const std::string & inMessage)
    {
        if (Element * logListBox = mLoggerApp->getElementById("logListBox"))
        {
            AttributesMapping attr;
            attr["label"] = inMessage;
            ElementPtr listItemPtr = ListItem::Create(logListBox, attr);
            listItemPtr->init();
        }
    }


} // namespace XULWin
