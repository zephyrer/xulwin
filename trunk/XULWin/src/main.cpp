#include "XULWin/Element.h"
#include "XULWin/Element.h"
#include "XULWin/ElementFactory.h"
#include "XULWin/ImageElement.h"
#include "XULWin/Initializer.h"
#include "XULWin/XULRunner.h"
#include "XULWin/ErrorReporter.h"
#include "XULWin/WinUtils.h"
#include "Poco/Path.h"
#include "Poco/StringTokenizer.h"
#include <windows.h>
#include <commctrl.h>


using namespace XULWin;


void runXUL(const std::string & inAppName)
{
    std::string chdir = "../xulrunnersamples/" + inAppName + "/";
    Windows::CurrentDirectoryChanger curdir(chdir);

    system("run.bat");

    XULRunner runner;
    runner.run("application.ini");
}


void runNoXULSample()
{
    AttributesMapping attr;
    ElementPtr window = WindowElement::Create(0, attr);
    ElementPtr vbox = VBoxElement::Create(window.get(), attr);

    ElementPtr hbox1 = HBoxElement::Create(vbox.get(), attr);

    attr["value"] = "Username:";
    ElementPtr label = LabelElement::Create(hbox1.get(), attr);
    attr.clear();

    attr["flex"] = "1";
    ElementPtr text = TextBoxElement::Create(hbox1.get(), attr);
    attr.clear();

    ElementPtr hbox2 = HBoxElement::Create(vbox.get(), attr);

    attr["value"] = "Password:";
    ElementPtr passLabel = LabelElement::Create(hbox2.get(), attr);
    attr.clear();

    attr["flex"] = "1";
    ElementPtr passText = TextBoxElement::Create(hbox2.get(), attr);
    attr.clear();

    window->downcast<WindowElement>()->showModal(WindowElement::Center);
    
}


void log(const std::string & inMessage)
{
    //MessageBoxA(0, inMessage.c_str(), "XULWin Logger", MB_OK);
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    // Initialize all kinds of stuff
    XULWin::Initializer initializer(hInstance);

    // Ensure that the common control DLL is loaded. 
    Windows::CommonControlsInitializer ccInit;


    ErrorReporter::Instance().setLogger(boost::bind(&log, _1));
    registerTypes(hInstance);
    runXUL("hello");
    runXUL("widgets");
    runXUL("configpanel");
    return 0;
}
