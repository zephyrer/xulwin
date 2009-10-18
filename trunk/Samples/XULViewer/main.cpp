#include "XULWin/XULRunner.h"
#include "XULWin/Decorator.h"
#include "XULWin/EventListener.h"
#include "XULWin/Element.h"
#include "XULWin/ElementImpl.h"
#include "XULWin/Initializer.h"
#include "XULWin/WinUtils.h"
#include "XULWin/Unicode.h"
#include <windows.h>


void reportError(const std::string & inMessage)
{
    std::wstring utf16Message = XULWin::ToUTF16(inMessage);
    ::MessageBox(0, utf16Message.c_str(), 0, MB_OK);
}


LRESULT runXUL(const std::string & inXULDocument)
{
    XULWin::XULRunner runner;
    XULWin::ElementPtr rootElement = runner.loadXUL(inXULDocument);
    if (!rootElement)
    {
        reportError("File not found or parser error.");
        return 1;
    }

    XULWin::NativeWindow * wnd = rootElement->impl()->downcast<XULWin::NativeWindow>();
    if (!wnd)
    {
        reportError("Root element is not of type window");
        return 1;
    }
    
    wnd->showModal(XULWin::Window::DefaultPosition);
    return 0;
}


void runXULViewer()
{
    XULWin::XULRunner runner;
    XULWin::ElementPtr rootElement = runner.loadXUL("XULViewer.xul");
    if (!rootElement)
    {
        reportError("No root element");
        return;
    }


    XULWin::Element * runButton = rootElement->getElementById("runButton");
    if (!runButton)
    {
        reportError("Run button not found");
        return;
    }
    XULWin::Element * pathBox = rootElement->getElementById("pathBox");
    if (!pathBox)
    {
        reportError("Path textbox not found");
        return;
    }

    XULWin::ScopedEventListener events;
    events.connect(runButton,
                   boost::bind(&runXUL,
                               boost::bind(&XULWin::Element::getAttribute,
                                           pathBox,
                                           "value")));

    if (XULWin::NativeWindow * wnd = rootElement->impl()->downcast<XULWin::NativeWindow>())
    {        
        wnd->showModal(XULWin::Window::CenterInScreen);
    }
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    // Initialize XULWin
    XULWin::Initializer initializer(hInstance);

    // Ensure that the common control DLL is loaded. 
    XULWin::Windows::CommonControlsInitializer ccInit;

    runXULViewer();
    return 0;
}