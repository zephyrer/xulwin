/**
 *  Tutorial 2: Event Handling and Loading External DTDs.
 *
 *  This tutorial illustrates how to handle events using the ScopedEventListener class.
 *  The code is found in ConfigSample.h and ConfigSample.cpp.
 *
 *  This sample also show how to read the GUI strings from an external DTD document.
 *  Since the default locale is "en-US", XULWin loads DTD in from the
 *  "chrome/locale/en-US" directory. A different local can be specified with the
 *  XULRunner::SetLocale function.
 *
 *  Checklist:
 *  1. Set code generation settings:
 *      - "Multi-threaded Debug" (/MTd) for Debug builds
 *      - "Multi-threaded" (/MT) for Release builds
 *  2. Link with: comctl32.lib, GdiPlus.lib.
 *  3. The project must have a manifest file.
 *      - See "Tutorial_02_Event_Handling_And_External_DTDs.exe.manifest"
 *  4. Set the debugging settings correctly
 *      Project settings -> Debugging -> Command Arguments: $(ProjectDir)ConfigPanel
 */
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


/**
 * Usage: Tutorial_02.exe XULApplicationPath
 */
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
