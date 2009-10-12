#include "XULWin/Lua/LuaBindings.h"
#include <windows.h> // TODO: remove


namespace XULWin
{

namespace Lua
{


    void ShowMessage(const std::string & inString)
    {
        ::MessageBoxA(0, inString.c_str(), "Lua", MB_OK);
    }


    Element::Element()
    {
    }

    
    int Element::testSum(int a, int b)
    {
        return a + b;
    }
    
    
    Element * Element::getElementById()
    {
        return 0;
    }


} // namespace Lua

} // namespace XULWin
