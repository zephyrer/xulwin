#include "Tester.h"
#include "Config.h"
#include "XULWin/Element.h"
#include "XULWin/Label.h"
#include "XULWin/Unicode.h"
#include "XULWin/XMLWindow.h"
#include "XULWin/XULRunner.h"
#include "XULWin/WinUtils.h"
#include "Poco/Path.h"
#include <boost/bind.hpp>


namespace XULWin
{

    Tester::Tester(HMODULE inModuleHandle, const std::string & inPathToXULRunnerSamples, Features inFeatures) :
        mModuleHandle(inModuleHandle),
        mPathToXULRunnerSamples(inPathToXULRunnerSamples),
        mFeatures(inFeatures)
    {
    }


    void Tester::runXULSample(const std::string & inAppName) const
    {
        // Change the current directory to the application dir.
        Poco::Path path(mPathToXULRunnerSamples);
        path.append(inAppName);
        WinAPI::CurrentDirectoryChanger curdir(path.toString());

        if (mFeatures & Features_EnableJavaScript)
        {
            XULRunner runner(mModuleHandle);
            runner.run("application.ini");
        }
        else
        {
            XULRunner runner(mModuleHandle);
            runner.run("application.ini");
        }

        // The application dir should contain a run.bat file.
        if (mFeatures & Features_TestWithMozillaXULRunner)
        {
            system("run.bat");
        }
    }


} // namespace XULWin
