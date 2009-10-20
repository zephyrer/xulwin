#include "Tester.h"
#include "TwitterTest.h"
#include "ConfigSample.h"
#include "LuaBindingsTest.h"
#include <boost/bind.hpp>
#include <sstream>
#include <windows.h>
#include <commctrl.h>


using namespace XULWin;


void log(const std::string & inMessage)
{
    //MessageBoxA(0, inMessage.c_str(), "XULWin Logger", MB_OK);
}


void runConfigSample()
{
    ConfigSample test;
    test.run();
}


void runImageViewerSample()
{
    ImageViewerSample sample;
    sample.run();
}


void startTest(XULWin::LuaBindingsTest & tester)
{
    //runConfigSample();
    //runImageViewerSample();    
    //tester.runXULSample("hello");
    //tester.runXULSample("MainWindow");
    //tester.runXULSample("listbox-simple");
    //tester.runXULSample("listbox-advanced");
    //tester.runXULSample("toolbar");
    //tester.runXULSample("widgets");
    //tester.runXULSample("tabbox");
    tester.runXULSample("treeview");
    //tester.runXULSample("configpanel");
    //tester.runXULSample("shout");
    //tester.runXULSample("svg");
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    // Initialize all kinds of stuff
    XULWin::Initializer initializer(hInstance);

    // Ensure that the common control DLL is loaded. 
    Windows::CommonControlsInitializer ccInit;

    XULWin::LuaBindingsTest tester;
    
    startTest(tester);
    //TwitterTest twitterTest;
    return 0;
}
