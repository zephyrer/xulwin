#ifndef XULRUNNERWITHLUA_H_INCLUDED
#define XULRUNNERWITHLUA_H_INCLUDED


#include "XULWin/Element.h"
#include "XULWin/EventListener.h"
#include "XULWin/Windows.h"
#include <boost/scoped_ptr.hpp>
#include <string>

struct lua_State;

namespace XULWin
{

    
class XULRunner;


namespace Lua
{

    /**
     * Lua::XULRunnerWithLua bundles a XULWin::XULRunner and a Lua state.
     * You should use a separate XULRunnerWithLua object for each XUL document
     * that you want to parse. This ensures that a separate Lua state will be
     * created per XUL document.
     */
    class XULRunnerWithLua : public EventListener
    {
    public:
        XULRunnerWithLua(HMODULE hModuleHandle);

        ~XULRunnerWithLua();

        ElementPtr loadApplication(const std::string & inApplicationIniFile);

        ElementPtr loadXUL(const std::string & inXULUrl);

        ElementPtr rootElement() const;

        boost::function<void(const std::string &)> Logger;

        bool loadFile(const std::string & inLuaFile);

        bool loadScript(const std::string & inScript);

        XULRunnerWithLua * getParentXULRunner();

        HMODULE getModuleHandle() const;

    private:

        virtual LRESULT handleCommand(Element * inSender, WORD inNotificationCode);        
        virtual LRESULT handleMenuCommand(Element * inSender, WORD inMenuId);        
        virtual LRESULT handleDialogCommand(Element * inSender, WORD inNotificationCode, WPARAM wParam, LPARAM lParam);
        virtual LRESULT handleMessage(Element * inSender, UINT inMessage, WPARAM wParam, LPARAM lParam);

        void log(const std::string & inMessage);

        void loadScripts(Element * inEl);

        void addListeners(Element * inEl);
        
        HMODULE mModuleHandle;
        boost::scoped_ptr<XULWin::XULRunner> mXULRunner;
        lua_State * mLuaState;
        XULRunnerWithLua * mPrevXULRunner;
    };


} // namespace Lua

} // namespace XULWin


#endif // XULRUNNERWITHLUA_H_INCLUDED
