#include "XULWin/Lua/LuaBindings.h"
#include "XULWin/ErrorReporter.h"
#include "XULWin/Initializer.h"
#include "XULWin/Unicode.h"
#include "XULWin/WinUtils.h"
#include "XULWin/Lua/XULRunnerWithLua.h"
#include "Poco/Path.h"
#include "Poco/Timer.h"
#include <windows.h> // TODO: remove


namespace XULWin
{


namespace Lua
{
    static XULRunnerWithLua * gActiveXULRunner(0);
    
    
    XULRunnerWithLua * setXULRunner(XULRunnerWithLua * inXULRunner)
    {
        XULRunnerWithLua * prev = gActiveXULRunner;
        gActiveXULRunner = inXULRunner;
        return prev;
    }


    Element * getRootElement()
    {
        if (gActiveXULRunner)
        {
            return gActiveXULRunner->rootElement().get();
        }
        return 0;
    }
    
    
    void setTimeout(const std::string & inCallback, int inMilliseconds)
    {
        assert(gActiveXULRunner);
        if (gActiveXULRunner)
        {
            Windows::setTimeout(boost::bind(&XULRunnerWithLua::loadScript, gActiveXULRunner, inCallback), inMilliseconds);
        }
    }


    void showMessage(const std::string & inString)
    {
        std::wstring utf16Message = ToUTF16(inString);
        ::MessageBox(0, utf16Message.c_str(), TEXT("Lua"), MB_OK);
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
    
    
    Dialog * toDialog(Element * inElement)
    {
        Dialog * result = 0;
        if (inElement)
        {
            result = inElement->downcast<Dialog>();
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
            if (Dialog * dlg = root->downcast<Dialog>())
            {         
                XULRunnerWithLua * parentXULRunner = runner.getParentXULRunner();
                assert(parentXULRunner);
                std::vector<Window *> windows;
                ElementPtr prevRoot = parentXULRunner->rootElement();
                prevRoot->getElementsByType<Window>(windows);
                if (!windows.empty())
                {
                    Window * wnd = windows[0];
                    if (wnd)
                    {
                        Element * textField = root->getElementById("textInput");
                        ::SetFocus(textField->impl()->downcast<NativeTextBox>()->handle());
                        dlg->showModal(wnd);
                        result = textField->getAttribute("value");
                    }
                }
            }
        }
        return result;
    }

} // namespace Lua

} // namespace XULWin
