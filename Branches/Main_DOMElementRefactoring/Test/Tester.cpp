#include "Tester.h"
#include "Config.h"
#include "XULWin/ComponentManager.h"
#include "XULWin/Element.h"
#include "XULWin/Unicode.h"
#include "XULWin/Window.h"
#include "XULWin/XULRunner.h"
#include "XULWin/WinUtils.h"
#include "XULWin/Js/JsException.h"
#include "XULWin/Js/JsXULRunner.h"
#include "Poco/Path.h"
#include <boost/bind.hpp>


namespace XULWin
{

    Tester::Tester(const std::string & inPathToXULRunnerSamples, Features inFeatures) :
        mPathToXULRunnerSamples(inPathToXULRunnerSamples),
        mFeatures(inFeatures)
    {
    }


    void LogJavaScriptException(const Js::JsException & inException)
    {
        std::wstring utf16Message = ToUTF16(inException.message());
        ::MessageBoxW(0, utf16Message.c_str(), L"XULRunner Tester: JavaScript exception", MB_OK);
    }


    void Tester::runXULSample(const std::string & inAppName) const
    {
        // Change the current directory to the application dir.
        Poco::Path path(mPathToXULRunnerSamples);
        path.append(inAppName);
        Windows::CurrentDirectoryChanger curdir(path.toString());

        if (mFeatures & Features_EnableJavaScript)
        {
            Js::JsXULRunner runner;
            runner.setExceptionLogger(boost::bind(&LogJavaScriptException, _1));
            runner.run("application.ini");
        }
        else
        {
            XULRunner runner;
            Poco::XML::Document * document = runner.loadApplication("application.ini");
            if (!document)
            {
                throw std::runtime_error("Failed to load the application: " + inAppName + ".");
            }

            if (Window * win = GetComponent<Window>(document->documentElement()))
            {
                win->showModal(WindowPos_CenterInScreen);
            }
        }

        // The application dir should contain a run.bat file.
        if (mFeatures & Features_TestWithMozillaXULRunner)
        {
            system("run.bat");
        }
    }


} // namespace XULWin
