#include "XULWin/Js/JsXULRunner.h"
#include "XULWin/ComponentManager.h"
#include "XULWin/ElementUtilities.h"
#include "XULWin/Js/JsSimpleContext.h"
#include "XULWin/Component.h"
#include "XULWin/Decorator.h"
#include "XULWin/ErrorReporter.h"
#include "XULWin/Initializer.h"
#include "XULWin/Window.h"
#include "XULWin/Windows.h"
#include "XULWin/WinUtils.h"
#include "XULWin/XULRunner.h"
#include "Poco/DOM/Element.h"
#include "Poco/DOM/Node.h"
#include "Poco/DOM/NodeList.h"
#include "Poco/DOM/Document.h"
#include <boost/bind.hpp>
#include <sstream>
#include <map>


using namespace v8;


namespace XULWin
{


    namespace Js
    {


        JsXULRunner::JsXULRunner() :
            mXULRunner(new XULRunner)
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
            Poco::XML::Document * document = loadApplication(inApplicationIniFile);
            if (!document)
            {
                ReportError("Failed to load the application. Probably due to parser error.");
                return;
            }

            Window * window = GetComponent<Window>(document->documentElement());
            if (!window)
            {
                ReportError("Root element is not of type window.");
                return;
            }

            window->showModal(WindowPos_CenterInScreen);
        }


        void JsXULRunner::initializeJavaScript(Poco::XML::Element * inRootElement)
        {
            mSimpleContext.reset(new JsSimpleContext(inRootElement));
            loadScripts(inRootElement);
            addListeners(inRootElement);
        }


        Poco::XML::Document * JsXULRunner::loadApplication(const std::string & inApplicationIniFile)
        {
            Poco::XML::Document * result = mXULRunner->loadApplication(inApplicationIniFile);
            if (!result)
            {
                throw std::runtime_error("Failed to load: " + inApplicationIniFile);
            }
            initializeJavaScript(result->documentElement());
            return result;
        }


        Poco::XML::Document * JsXULRunner::loadXULFromFile(const std::string & inXULUrl)
        {
            Poco::XML::Document * result = mXULRunner->loadXULFromFile(inXULUrl);
            if (!result)
            {
                std::string msg = "Failed to load: " + inXULUrl + ".";
                throw std::runtime_error(msg.c_str());
            }
            initializeJavaScript(result->documentElement());
            return result;
        }


        Poco::XML::Document * JsXULRunner::loadXULFromString(const std::string & inXULString)
        {
            Poco::XML::Document * result = mXULRunner->loadXULFromString(inXULString);
            if (!result)
            {
                std::string msg = "Failed to parse the XULString. Reason:\n" + inXULString;
                throw std::runtime_error(msg.c_str());
            }
            initializeJavaScript(result->documentElement());
            return result;
        }


        Poco::XML::Document * JsXULRunner::document()
        {
            return mXULRunner->document();
        }


        Poco::XML::Element * JsXULRunner::rootElement() const
        {
            return mXULRunner->rootElement();
        }


        void JsXULRunner::loadScript(const std::string & inText)
        {
            if (!mSimpleContext)
            {
                throw std::logic_error("JavaScript is not yet initialized.");
            }
            mSimpleContext->loadScript(inText);            
        }


        void JsXULRunner::loadScripts(Poco::XML::Element * inElement)
        {
            Poco::XML::NodeList * nodeList = inElement->getElementsByTagName("script");
            if (nodeList->length() == 0)
            {
                return;
            }

            Poco::XML::Node * currentNode = nodeList->item(0);
            while (currentNode)
            {
                try
                {
                    loadScript(currentNode->innerText());
                }
                catch (const JsException & inException)
                {
                    logJsException(inException);
                }
                currentNode = currentNode->nextSibling();
            }
        }


        void JsXULRunner::addListeners(Poco::XML::Element * inElement)
        {
            //if (NativeComponent * comp = GetComponent<NativeComponent>(inElement))
            //{
            //    comp->addEventListener(this);
            //}

            //XULWin::ForEach(inElement->childNodes(),
            //                boost::bind(&JsXULRunner::addListeners, this, boost::bind(&Node2Element, _1)));
        }


        LRESULT JsXULRunner::handleCommand(Component * inSender, WORD inNotificationCode, WPARAM wParam, LPARAM lParam)
        {
            try
            {
                std::string oncommand = inSender->el()->getAttribute("oncommand");
                if (!oncommand.empty())
                {
                    loadScript(oncommand);
                    return EventResult_Handled;
                }
            }
            catch (const JsException & inException)
            {
                logJsException(inException);
            }
            return EventResult_NotHandled;
        }


        //LRESULT JsXULRunner::handleMenuCommand(Component * inSender, WORD inMenuId)
        //{
        //    return 1;
        //}


        LRESULT JsXULRunner::handleDialogCommand(Component * inSender, WORD inNotificationCode, WPARAM wParam, LPARAM lParam)
        {
            return 1;
        }


        LRESULT JsXULRunner::handleMessage(Component * inSender, UINT inMessage, WPARAM wParam, LPARAM lParam)
        {
            return 1;
        }


    } // namespace Js

} // namespace XULWin
