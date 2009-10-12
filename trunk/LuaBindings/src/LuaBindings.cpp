#include "XULWin/Lua/LuaBindings.h"
#include "XULWin/ErrorReporter.h"
#include "XULWin/Initializer.h"
#include "XULWin/WinUtils.h"
#include "XULWin/XULRunner.h"
#include <windows.h> // TODO: remove


namespace XULWin
{


namespace Lua
{
    XULWin::Initializer * gInitializer(0);

    static ElementPtr gRootElement;

    void showMessage(const std::string & inString)
    {
        ::MessageBoxA(0, inString.c_str(), "Lua", MB_OK);
    }
    
    
    void initialize()
    {
        gInitializer = new XULWin::Initializer(GetModuleHandle(0));
        Windows::CommonControlsInitializer ccInit;
        ErrorReporter::Instance().setLogger(boost::bind(&showMessage, _1));
    }


    void finalize()
    {
        gRootElement.reset();
        delete gInitializer;
        gInitializer = 0;
    }


    Element * loadApplication(const std::string & inFile)
    {
        Windows::CurrentDirectoryChanger cd("../xulrunnersamples/" + inFile + "/");
        XULWin::XULRunner runner;
        gRootElement = runner.loadApplication("application.ini");
        return gRootElement.get();
    }
    
    
    void showModal(Element * inWindow)
    {
        inWindow->impl()->downcast<NativeWindow>()->showModal();
    }


} // namespace Lua

} // namespace XULWin
