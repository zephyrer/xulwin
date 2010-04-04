#include "Tester.h"
#include "ConfigSample.h"
#include "XULWin/ErrorReporter.h"
#include "XULWin/Unicode.h"
#include "XULWin/Windows.h"
#include <boost/bind.hpp>
#include <sstream>
#include <commctrl.h>


using namespace XULWin;


void runConfigSample(HMODULE inModuleHandle, const std::string & inPathToXULRunnerSamples)
{
    ConfigSample test(inModuleHandle, inPathToXULRunnerSamples);
    test.run();
}


void runImageViewerSample(HMODULE inModuleHandle, const std::string & inPathToXULRunnerSamples)
{
    ImageViewerSample sample(inModuleHandle, inPathToXULRunnerSamples);
    sample.run();
} 


void startTest(HINSTANCE hInstance, Tester & tester, const std::string & inPathToXULRunnerSamples)
{
    //runConfigSample(hInstance, inPathToXULRunnerSamples);
    //runImageViewerSample(hInstance, inPathToXULRunnerSamples);    
    
    tester.runXULSample("configpanel");

    //tester.runXULSample("hello");
    //tester.runXULSample("MainWindow");
    //tester.runXULSample("listbox-simple");
    //tester.runXULSample("listbox-advanced");
    //tester.runXULSample("toolbar");
    //tester.runXULSample("widgets");
    //tester.runXULSample("tabbox");
    //tester.runXULSample("treeview");
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
        ::MessageBox(0, TEXT("Expected argument: path to the 'xulrunnersamples' directory.\nIn \"Project Settings\" -> \"Debugging\" -> \"Command Arguments\"\nset the value \"$(SolutionDir)xulrunnersamples\"."), 0, MB_OK);
        return 1;
    }

    // Initialize all kinds of stuff
    Initializer initializer(hInstance);

    // Ensure that the common control DLL is loaded. 
    Windows::CommonControlsInitializer ccInit;

    Tester tester(hInstance, commandLine, Tester::Features_EnableJavaScript);
    
    startTest(hInstance, tester, commandLine);
    return 0;
}
