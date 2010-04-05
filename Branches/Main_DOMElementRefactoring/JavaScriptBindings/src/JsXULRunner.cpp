#include "XULWin/Js/JsXULRunner.h"
#include "XULWin/Js/JsSimpleContext.h"
#include "XULWin/Component.h"
#include "XULWin/Decorator.h"
#include "XULWin/ErrorReporter.h"
#include "XULWin/Initializer.h"
#include "XULWin/ScriptElement.h"
#include "XULWin/Window.h"
#include "XULWin/Windows.h"
#include "XULWin/WinUtils.h"
#include "XULWin/XULRunner.h"
#include <sstream>
#include <map>


using namespace v8;


namespace XULWin
{


    namespace Js
    {


        JsXULRunner::JsXULRunner() :
            mXULRunner2(new XULRunner)
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


        void JsXULRunner::run(const std::string & inApplicationIniFile)
        {
            ElementPtr rootEl = loadApplication(inApplicationIniFile);
            if (!rootEl)
            {
                ReportError("Failed to load the application. Probably due to parser error.");
                return;
            }
                
            WindowElement * window = rootEl->downcast<WindowElement>();
            if (!window)
            {
                ReportError("Root element is not of type window.");
                return;
            }

            window->showModal(WindowElement::CenterInScreen);
        }


        void JsXULRunner::initializeJavaScript(Element * inRootElement)
        {
            mSimpleContext.reset(new JsSimpleContext(inRootElement));
            loadScripts(inRootElement);
            addListeners(inRootElement);
        }


        ElementPtr JsXULRunner::loadApplication(const std::string & inApplicationIniFile)
        {
            ElementPtr result = mXULRunner2->loadApplication(inApplicationIniFile);
            if (!result)
            {
                throw std::runtime_error("Failed to load: " + inApplicationIniFile);
            }
            initializeJavaScript(result.get());
            return result;
        }


        ElementPtr JsXULRunner::loadXULFromFile(const std::string & inXULUrl)
        {
            ElementPtr result = mXULRunner2->loadXULFromFile(inXULUrl);
            if (!result)
            {
                std::string msg = "Failed to load: " + inXULUrl + ".";
                throw std::runtime_error(msg.c_str());
            }
            initializeJavaScript(result.get());
            return result;
        }


        ElementPtr JsXULRunner::loadXULFromString(const std::string & inXULString)
        {
            ElementPtr result = mXULRunner2->loadXULFromString(inXULString);
            if (!result)
            {
                std::string msg = "Failed to parse the XULString. Reason:\n" + inXULString;
                throw std::runtime_error(msg.c_str());
            }
            initializeJavaScript(result.get());
            return result;
        }


        ElementPtr JsXULRunner::rootElement() const
        {
            return mXULRunner2->rootElement();
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
