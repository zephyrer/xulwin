#ifndef XULRUNNERWITHLUA_H_INCLUDED
#define XULRUNNERWITHLUA_H_INCLUDED


#include "XULWin/Element.h"
#include "XULWin/EventListener.h"
#include <boost/scoped_ptr.hpp>
#include <string>

struct lua_State;

namespace XULWin
{

    
class XULRunner;


namespace Lua
{

    class XULRunnerWithLua : public EventListener
    {
    public:
        XULRunnerWithLua();

        ~XULRunnerWithLua();

        ElementPtr loadApplication(const std::string & inApplicationIniFile);

        ElementPtr loadXUL(const std::string & inXULUrl);

    private:

        virtual LRESULT handleCommand(Element * inSender, WORD inNotificationCode);        
        virtual LRESULT handleMenuCommand(Element * inSender, WORD inMenuId);        
        virtual LRESULT handleDialogCommand(Element * inSender, WORD inNotificationCode, WPARAM wParam, LPARAM lParam);
        virtual LRESULT handleMessage(Element * inSender, UINT inMessage, WPARAM wParam, LPARAM lParam);

        bool loadFile(const std::string & inLuaFile);

        bool loadScript(const std::string & inScript);

        boost::function<void(const std::string &)> Logger;

        void log(const std::string & inMessage);

        void loadScripts(Element * inEl);

        void addListeners(Element * inEl);

        boost::scoped_ptr<XULWin::XULRunner> mXULRunner;
        lua_State * mLuaState;
    };


} // namespace Lua

} // namespace XULWin


#endif // XULRUNNERWITHLUA_H_INCLUDED
