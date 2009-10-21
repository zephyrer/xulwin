#include "LuaBindingsTest.h"
#include "Config.h"
#include "XULWin/Element.h"
#include "XULWin/ErrorReporter.h"
#include "XULWin/ListItemElement.h"
#include "XULWin/Unicode.h"
#include "XULWin/WinUtils.h"
#include "XULWin/Lua/LuaBindings.h"
#include "XULWin/Lua/XULRunnerWithLua.h"
#include "Poco/Path.h"
#include <boost/bind.hpp>


namespace XULWin
{


    LuaBindingsTest::LuaBindingsTest(HINSTANCE hInstance, const std::string & inPathToXULRunnerSamples) :
        mPathToXULRunnerSamples(inPathToXULRunnerSamples)
    {    
        Poco::Path loggerPath(Windows::getApplicationDirectory(hInstance));
        loggerPath.append("Logger.xul");
        mLoggerApp = mLoggerRunner.loadXUL(loggerPath.toString());
        if (WindowElement * wnd = mLoggerApp->downcast<WindowElement>())
        {
            wnd->component()->move(0, 500, 400, 400);
            wnd->show(WindowElement::DefaultPosition);
        }
        ErrorReporter::Instance().setLogger(boost::bind(&LuaBindingsTest::log, this, _1));
        ErrorCatcher errorCatcher;
        ReportError("Test logger"); 
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

        Lua::XULRunnerWithLua xulRunner;
        xulRunner.Logger = boost::bind(Lua::showMessage, _1);
        ElementPtr rootEl = xulRunner.loadApplication("application.ini");
        if (!rootEl)
        {
            #if FORCE_MESSAGEBOX_LOGGING
            std::wstring message = XULWin::ToUTF16(appPath.toString() + " not found");
            ::MessageBox(0, message.c_str(), 0, MB_OK);
            #endif

            ReportError("Failed to load XUL sample: " + inAppname);
            return;
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
