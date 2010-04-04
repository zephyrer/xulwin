#include "XULWin/Js/JsXULRunner.h"
#include "XULWin/ErrorReporter.h"
#include "XULWin/Component.h"
#include "XULWin/Decorator.h"
#include "XULWin/Initializer.h"
#include "XULWin/ScriptElement.h"
#include "XULWin/Windows.h"
#include "XULWin/WinUtils.h"
#include "XULWin/XULRunner.h"
#include "XULWin/Js/JsSimpleContext.h"
#include <sstream>
#include <map>


using namespace v8;


namespace XULWin
{


    namespace Js
    {


        JsXULRunner::JsXULRunner(HMODULE inModuleHandle) :
            mModuleHandle(inModuleHandle),
            mXULRunner(new XULRunner(inModuleHandle)),
            mSimpleContext(new JsSimpleContext)
        {
        }


        JsXULRunner::~JsXULRunner()
        {
        }
       

        void JsXULRunner::setExceptionLogger(const JsExceptionLogger & inLogger)
        {
            mLogger = inLogger;
        }

        
        void JsXULRunner::logJsException(const JsException & inJsException)
        {
            if (mLogger)
            {
                mLogger(inJsException);
            }
        }


        HMODULE JsXULRunner::getModuleHandle() const
        {
            return mXULRunner->getModuleHandle();
        }


        ElementPtr JsXULRunner::loadApplication(const std::string & inApplicationIniFile)
        {
            ElementPtr result = mXULRunner->loadApplication(inApplicationIniFile);
            if (result)
            {
                loadScripts(result.get());
                addListeners(result.get());
            }
            else
            {
                ReportError("Failed to load: " + inApplicationIniFile);
            }
            return result;
        }


        ElementPtr JsXULRunner::loadXUL(const std::string & inXULUrl)
        {
            ElementPtr result = mXULRunner->loadXUL(inXULUrl);
            if (result)
            {
                loadScripts(result.get());
                addListeners(result.get());
            }
            return result;
        }


        ElementPtr JsXULRunner::rootElement() const
        {
            return mXULRunner->rootElement();
        }


        void JsXULRunner::loadScripts(Element * inElement)
        {
            std::vector<XULWin::ScriptElement *> scripts;
            inElement->getElementsByType<ScriptElement>(scripts);
            for (size_t idx = 0; idx != scripts.size(); ++idx)
            {
                try
                {
                    if (mSimpleContext)
                    {
                        mSimpleContext->loadScript(scripts[idx]->innerText());
                    }
                }
                catch (const JsException & inException)
                {
                    logJsException(inException);
                }
            }
        }


        void JsXULRunner::addListeners(Element * inElement)
        {
            if (NativeComponent * comp = inElement->component()->downcast<NativeComponent>())
            {
                comp->addEventListener(this);
            }
            for (size_t idx = 0; idx != inElement->children().size(); ++idx)
            {
                addListeners(inElement->children()[idx].get());
            }
        }


        LRESULT JsXULRunner::handleCommand(Element * inSender, WORD inNotificationCode, WPARAM wParam, LPARAM lParam)
        {
            try
            {
                std::string oncommand = inSender->getAttribute("oncommand");
                if (!oncommand.empty())
                {
                    if (mSimpleContext)
                    {
                        mSimpleContext->loadScript(oncommand);
                        return 0;
                    }
                }
            }
            catch (const JsException & inException)
            {
                logJsException(inException);
            }
            return 1;
        }


        LRESULT JsXULRunner::handleMenuCommand(Element * inSender, WORD inMenuId)
        {
            return 1;
        }


        LRESULT JsXULRunner::handleDialogCommand(Element * inSender, WORD inNotificationCode, WPARAM wParam, LPARAM lParam)
        {
            return 1;
        }


        LRESULT JsXULRunner::handleMessage(Element * inSender, UINT inMessage, WPARAM wParam, LPARAM lParam)
        {
            return 1;
        }


    } // namespace Js

} // namespace XULWin
