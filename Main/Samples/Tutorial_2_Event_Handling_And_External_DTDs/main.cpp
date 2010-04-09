//
// Tutorial 2: Event Handling and Loading External DTDs.
//
// This tutorial illustrates how to handle events using the ScopedEventListener class.
// See ConfigSample.cpp.
//
// This sample also loads the UI strings from an external DTD document. It loads the
// DTD found in the chrome/locale/en-US folder. en-US is the default locale. If you 
// want to change it then you can use the XULRunner::SetLocale function.
//
// Feel free to experiment.
//
#include "ConfigSample.h"
#include <sstream>
#include <string>



// Sometimes Windows wraps it's arguments in quotes, this function removes them.
std::string unquote(const std::string & inString)
{
    if (inString.size() >= 2 && inString[0] == '"' && inString[inString.size() - 1] == '"')
    {
        return inString.substr(1, inString.size() - 2);
    }
    return inString;
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    // Initialize the XULWin library
    XULWin::Initializer initializer(hInstance);

    // Get the application dir from command line args
    // The application directory is the directory that contains the application.ini file.
    std::string appDir(unquote(lpCmdLine));
    if (appDir.empty())
    {
        ::MessageBox(0, TEXT("Please set the command arguments correctly:\nTutorial Project settings -> Debugging -> Command Arguments: $(ProjectDir)ConfigPanel"), 0, MB_OK);
        return 1;
    }

    XULWin::ConfigSample configSample(hInstance, appDir);
    configSample.run();
    return 0;
}
