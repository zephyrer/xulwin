#include "Tester.h"
#include "ConfigSample.h"
#include "LuaBindingsTest.h"
#include "XULWin/ErrorReporter.h"
#include "XULWin/Unicode.h"
#include "XULWin/Windows.h"
#include <boost/bind.hpp>
#include <sstream>
#include <commctrl.h>


using namespace XULWin;


void runConfigSample(const std::string & inPathToXULRunnerSamples)
{
    ConfigSample test(inPathToXULRunnerSamples);
    test.run();
}


void runImageViewerSample(const std::string & inPathToXULRunnerSamples)
{
    ImageViewerSample sample(inPathToXULRunnerSamples);
    sample.run();
} 


void startTest(XULWin::LuaBindingsTest & tester, const std::string & inPathToXULRunnerSamples)
{
    //runConfigSample(inPathToXULRunnerSamples);
    //runImageViewerSample(inPathToXULRunnerSamples);    
    //tester.runXULSample("hello");
    //tester.runXULSample("MainWindow");
    //tester.runXULSample("listbox-simple");
    //tester.runXULSample("listbox-advanced");
    //tester.runXULSample("toolbar");
    tester.runXULSample("widgets");
    //tester.runXULSample("tabbox");
    //tester.runXULSample("treeview");
    //tester.runXULSample("configpanel");
    //tester.runXULSample("shout");
    //tester.runXULSample("svg");
}


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
    std::string commandLine(unquote(lpCmdLine));
    if (commandLine.empty())
    {
        ::MessageBox(0, TEXT("Expected argument: path to the 'xulrunnersamples' directory."), 0, MB_OK);
        return 1;
    }

    // Initialize all kinds of stuff
    XULWin::Initializer initializer(hInstance);

    // Ensure that the common control DLL is loaded. 
    Windows::CommonControlsInitializer ccInit;

    XULWin::LuaBindingsTest tester(hInstance, commandLine);
    
    startTest(tester, commandLine);
    return 0;
}
