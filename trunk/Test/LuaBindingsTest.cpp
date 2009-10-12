#include "LuaBindingsTest.h"
#include "LuaInitializer.h"
#include "XULWin/Element.h"
#include "XULWin/ErrorReporter.h"
#include "XULWin/Script.h"
#include "XULWin/WinUtils.h"
#include "XULWin/XULRunner.h"
#include "XULWin/Lua/LuaBindings.h"
#include <boost/bind.hpp>
#include <lua.hpp>
#include <stdio.h>
#include <windows.h> // TODO: remove?


namespace XULWin
{

    void DisplayError(const std::string & inMessage)
    {
        ::MessageBoxA(0, ("Error: " + inMessage).c_str(), "XULWin", MB_OK);
    }


    LuaBindingsTest::LuaBindingsTest()
    {
        mInitializer.reset(new Lua::LuaInitializer);
        mInitializer->Logger = boost::bind(&DisplayError, _1);
    }


    LuaBindingsTest::~LuaBindingsTest()
    {
        mInitializer.reset();
    }


    void LuaBindingsTest::addListeners(Element * inElement)
    {
        if (NativeComponent * comp = inElement->impl()->downcast<NativeComponent>())
        {
            comp->addEventListener(this);
        }
        for (size_t idx = 0; idx != inElement->children().size(); ++idx)
        {
            addListeners(inElement->children()[idx].get());
        }
    }


    void LuaBindingsTest::run()
    {
        XULRunner runner;
        Windows::CurrentDirectoryChanger cd("../xulrunnersamples/hello/");
        mRootElement = runner.loadApplication("application.ini");
        Lua::setRootElement(mRootElement.get());

        std::vector<XULWin::Script*> scripts;
        mRootElement->getElementsByType<Script>(scripts);
        for (size_t idx = 0; idx != scripts.size(); ++idx)
        {
            mInitializer->loadScript(scripts[idx]->innerText());
        }
        addListeners(mRootElement.get());

        // Show the window
        mRootElement->impl()->downcast<NativeWindow>()->showModal();
    }


    LRESULT LuaBindingsTest::handleCommand(Element * inSender, WORD inNotificationCode)
    {
        std::string script = inSender->getAttribute("oncommand");
        if (!script.empty() && mInitializer->loadScript(script))
        {
            return 0;
        }
        return 1;
    }
    
    
    LRESULT LuaBindingsTest::handleMenuCommand(Element * inSender, WORD inMenuId)
    {
        return 1;
    }


    LRESULT LuaBindingsTest::handleDialogCommand(Element * inSender, WORD inNotificationCode, WPARAM wParam, LPARAM lParam)
    {
        return 1;
    }


    LRESULT LuaBindingsTest::handleMessage(Element * inSender, UINT inMessage, WPARAM wParam, LPARAM lParam)
    {
        return 1;
    }


} // namespace XULWin
