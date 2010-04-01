#include "LuaBindingsTest.h"
#include "Config.h"
#include "XULWin/Element.h"
#include "XULWin/ErrorReporter.h"
#include "XULWin/ListItemElement.h"
#include "XULWin/Unicode.h"
#include "XULWin/WinUtils.h"
#include "XULWin/WindowElement.h"
#include "XULWin/Lua/LuaBindings.h"
#include "XULWin/Lua/XULRunnerWithLua.h"
#include "Poco/Path.h"
#include <boost/bind.hpp>
#include <sstream>


namespace XULWin
{


    LuaBindingsTest::LuaBindingsTest(HMODULE inModuleHandle, const std::string & inPathToXULRunnerSamples) :
        mModuleHandle(inModuleHandle),
        mLoggerRunner(inModuleHandle),
        mPathToXULRunnerSamples(inPathToXULRunnerSamples)
    {    
        Poco::Path loggerPath(Windows::getApplicationDirectory(inModuleHandle));
        loggerPath.append("Logger.xul");
        mLoggerApp = mLoggerRunner.loadXUL(loggerPath.toString());
        if (WindowElement * wnd = mLoggerApp->downcast<WindowElement>())
        {
            wnd->component()->move(0, 500, 400, 400);
            wnd->show(WindowElement::DefaultPosition);
        }
        ErrorReporter::Instance().setLogger(boost::bind(&LuaBindingsTest::log, this, _1));
    }


    LuaBindingsTest::~LuaBindingsTest()
    {
        ErrorReporter::Instance().setLogger(ErrorReporter::LogFunction());
    }


    void LuaBindingsTest::runXULSample(const std::string & inAppname)
    {
        Poco::Path appPath(mPathToXULRunnerSamples, inAppname);
        Windows::CurrentDirectoryChanger cd(appPath.toString());

#if TEST_WITH_MOZILLA_XULRUNNER
        ::ShellExecute(NULL, TEXT("open"), TEXT("run.bat"), NULL, NULL, SW_SHOWNORMAL);
#endif
        Lua::XULRunnerWithLua xulRunner(mModuleHandle);
        ElementPtr rootEl;

        // Extra scope added to have the ErrorCatcher to do its logging on end of scope
        {
            ErrorCatcher errorCatcher;
            xulRunner.Logger = boost::bind(Lua::showMessage, _1);
            rootEl = xulRunner.loadApplication("application.ini");

            if (!rootEl)
            {
                ReportError("Failed to load XUL sample: " + inAppname);
                return;
            }
        }

        if (WindowElement * wnd = rootEl->downcast<WindowElement>())
        {
            wnd->showModal(WindowElement::CenterInScreen);
        }
    }
    
    
    void LuaBindingsTest::log(const std::string & inMessage)
    {
        if (Element * logListBox = mLoggerApp->getElementById("logListBox"))
        {
            AttributesMapping attr;
            attr["label"] = inMessage;
            ElementPtr listItemPtr = ListItemElement::Create(logListBox, attr);
            listItemPtr->init();
        }
    }


} // namespace XULWin
