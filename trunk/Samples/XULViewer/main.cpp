#include "XULWin/XULRunner.h"
#include "XULWin/Decorator.h"
#include "XULWin/EventListener.h"
#include "XULWin/Element.h"
#include "XULWin/Component.h"
#include "XULWin/ErrorReporter.h"
#include "XULWin/Initializer.h"
#include "XULWin/WinUtils.h"
#include "XULWin/Unicode.h"
#include <windows.h>
#include <shellapi.h>
#include <sstream>


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
    
    wnd->rebuildLayout();
    wnd->showModal(XULWin::Window::DefaultPosition);
    return 0;
}


LRESULT dropFiles(XULWin::Element * inRootElement, WPARAM wParam, LPARAM lParam)
{
    int numFiles = ::DragQueryFile((HDROP)wParam, 0xFFFFFFFF, 0, 0);
    for (int idx = 0; idx < numFiles; ++idx)
    {
        // Get filename and run XUL
    	TCHAR fileName[MAX_PATH];
        ::DragQueryFile((HDROP)wParam, idx, &fileName[0], MAX_PATH);
        runXUL(XULWin::ToUTF8(&fileName[0]));
        break; // only consider first file
    }
    return 0;
}


void runXULViewer()
{
    XULWin::ErrorCatcher errorCatcher;
    XULWin::XULRunner runner;
    XULWin::ElementPtr rootElement = runner.loadXUL("XULViewer.xul");
    if (errorCatcher.hasCaught())
    {
        std::stringstream ss;
        errorCatcher.getErrorMessage(ss);
        reportError(ss.str());
        return;
    }

    XULWin::NativeWindow * wnd = rootElement->impl()->downcast<XULWin::NativeWindow>();
    if (!wnd)
    {
        reportError("Root element is not of type winodw.");
        return;
    }


    // Accept drag and drop of files
    ::DragAcceptFiles(wnd->handle(), TRUE);

    // Connect to WM_DROPFILES message
    XULWin::ScopedEventListener events;
    events.connect(wnd->el(),
                   WM_DROPFILES,
                   boost::bind(&dropFiles, rootElement.get(), _1, _2));
    wnd->showModal(XULWin::Window::CenterInScreen);
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