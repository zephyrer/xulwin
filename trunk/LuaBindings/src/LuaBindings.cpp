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
    static Element * gRootElement(0);
    
    
    Element * setRootElement(Element * inEl)
    {
        Element * prev = gRootElement;
        gRootElement = inEl;
        return prev;
    }


    Element * getRootElement()
    {
        return gRootElement;
    }


    void showMessage(const std::string & inString)
    {
        ::MessageBoxA(0, inString.c_str(), "Lua", MB_OK);
    }


} // namespace Lua

} // namespace XULWin
