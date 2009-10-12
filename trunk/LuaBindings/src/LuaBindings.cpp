#include "XULWin/Lua/LuaBindings.h"
#include "XULWin/ErrorReporter.h"
#include "XULWin/Initializer.h"
#include "XULWin/WinUtils.h"
#include "XULWin/Lua/XULRunnerWithLua.h"
#include "Poco/Path.h"
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
    
    
    Window * toWindow(Element * inElement)
    {
        Window * result = 0;
        if (inElement)
        {
            result = inElement->downcast<Window>();
        }
        return result;
    }


    std::string prompt(const std::string & inText, const std::string & inDefault)
    {
        std::string result = inDefault;
        XULRunnerWithLua runner;
        Poco::Path promptFile(__FILE__);
        std::string path = promptFile.parent().parent().append("Prompt.xul").toString();
        ElementPtr root = runner.loadXUL(path);
        if (root)
        {
            if (Window * win = root->downcast<Window>())
            {
                win->showModal();
                if (Element * textField = root->getElementById("textInput"))
                {
                    result = textField->getAttribute("value");
                }
            }
        }
        return result;
    }

} // namespace Lua

} // namespace XULWin
