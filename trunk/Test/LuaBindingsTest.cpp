#include "LuaBindingsTest.h"
#include "LuaInitializer.h"
#include "XULWin/ErrorReporter.h"
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


    LRESULT LuaBindingsTest::handleButtonPressed()
    {
        // Execute the script found in the 'oncommand' attribute of the button.
        Element * button = mRootElement->getElementById("helloButton");
        if (mInitializer->loadScript(button->getAttribute("oncommand")))
        {
            return 0;
        }
        return 1;
    }


    void LuaBindingsTest::run()
    {
        XULRunner runner;
        Windows::CurrentDirectoryChanger cd("../xulrunnersamples/hello/");
        mRootElement = runner.loadApplication("application.ini");
        Lua::setRootElement(mRootElement.get());

        // Load the script
        Element * script = mRootElement->getElementById("sayHelloScript");
        mInitializer->loadScript(script->innerText());
        
        // Connect the button listener
        Element * button = mRootElement->getElementById("helloButton");        
        ScopedEventListener events;
        events.connect(button, boost::bind(&LuaBindingsTest::handleButtonPressed, this));

        // Show the window
        mRootElement->impl()->downcast<NativeWindow>()->showModal();
    }


} // namespace XULWin
