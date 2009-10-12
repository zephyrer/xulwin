#ifndef LUABINDINGSTEST_H_INCLUDED
#define LUABINDINGSTEST_H_INCLUDED


#include "XULWin/Lua/LuaBindings.h"
#include "XULWin/Element.h"
#include "XULWin/EventListener.h"
#include "XULWin/Parser.h"
#include <boost/scoped_ptr.hpp>
#include <string>


namespace XULWin
{

    namespace Lua
    {
        class LuaInitializer;
    }


    class LuaBindingsTest : public EventListener
    {
    public:
        LuaBindingsTest();

        ~LuaBindingsTest();

        void run();

    private:
        virtual LRESULT handleCommand(Element * inSender, WORD inNotificationCode);
        virtual LRESULT handleMenuCommand(Element * inSender, WORD inMenuId);
        virtual LRESULT handleDialogCommand(Element * inSender, WORD inNotificationCode, WPARAM wParam, LPARAM lParam);
        virtual LRESULT handleMessage(Element * inSender, UINT inMessage, WPARAM wParam, LPARAM lParam);

        void addListeners(Element * inElement);

        LRESULT handleButtonPressed();
        boost::scoped_ptr<Lua::LuaInitializer> mInitializer;
        ElementPtr mRootElement;
    };


} // namespace XULWin


#endif // LUABINDINGSTEST_H_INCLUDED
