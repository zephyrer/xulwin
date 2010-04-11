#include "XULWin/XULRunner.h"
#include "XULWin/Decorator.h"
#include "XULWin/EventListener.h"
#include "XULWin/Element.h"
#include "XULWin/Component.h"
#include "XULWin/ErrorReporter.h"
#include "XULWin/Initializer.h"
#include "XULWin/Window.h"
#include "XULWin/WinUtils.h"
#include "XULWin/Unicode.h"
#include <windows.h>
#include <shellapi.h>
#include <sstream>


LRESULT runXUL(HMODULE inModuleHandle, const std::string & inXULDocument)
{
	XULWin::ErrorCatcher errorCatcher;
    XULWin::XULRunner runner(inModuleHandle);
    XULWin::ElementPtr rootElement = runner.loadXULFromFile(inXULDocument);
    if (!rootElement)
    {
        XULWin::ReportError("Failed to load the XUL document.");
        return 1;
    }

    XULWin::Window * wnd = rootElement->component()->downcast<XULWin::Window>();
    if (!wnd)
    {
        XULWin::ReportError("Root element is not of type window");
        return 1;
    }
    
    wnd->rebuildLayout();
    wnd->showModal(XULWin::WindowElement::DefaultPosition);
    return 0;
}


LRESULT dropFiles(HMODULE inModuleHandle, XULWin::Element * inRootElement, WPARAM wParam, LPARAM lParam)
{
    int numFiles = ::DragQueryFile((HDROP)wParam, 0xFFFFFFFF, 0, 0);
    for (int idx = 0; idx < numFiles; ++idx)
    {
        // Get filename and run XUL
    	TCHAR fileName[MAX_PATH];
        ::DragQueryFile((HDROP)wParam, idx, &fileName[0], MAX_PATH);
        runXUL(inModuleHandle, XULWin::ToUTF8(&fileName[0]));
        break; // only consider first file
    }
    return 0;
}


void runXULViewer(HMODULE inModuleHandle)
{
    XULWin::ErrorCatcher errorCatcher;
    XULWin::XULRunner runner(inModuleHandle);
    XULWin::ElementPtr rootElement = runner.loadXULFromFile("XULViewer.xul");
    if (!rootElement)
    {
        XULWin::ReportError("Failed to load the root element");
        return;
    }

    XULWin::Window * wnd = rootElement->component()->downcast<XULWin::Window>();
    if (!wnd)
    {
        XULWin::ReportError("Root element is not of type winodw.");
        return;
    }


    // Accept drag and drop of files
    ::DragAcceptFiles(wnd->handle(), TRUE);

    // Connect to WM_DROPFILES message
    XULWin::ScopedEventListener events;
    events.connect(wnd->el(),
                   WM_DROPFILES,
                   boost::bind(&dropFiles, inModuleHandle, rootElement.get(), _1, _2));
    wnd->showModal(XULWin::WindowElement::CenterInScreen);
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    // Initialize XULWin
    XULWin::Initializer initializer(hInstance);

    // Ensure that the common control DLL is loaded. 
    XULWin::Windows::CommonControlsInitializer ccInit;

    runXULViewer(hInstance);
    return 0;
}
