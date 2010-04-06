#ifndef XULRUNNERJS_H_INCLUDED
#define XULRUNNERJS_H_INCLUDED


#include "XULWin/Element.h"
#include "XULWin/EventListener.h"
#include "XULWin/Windows.h"
#include <boost/function.hpp>
#include <boost/scoped_ptr.hpp>
#include <string>


namespace Poco
{
    namespace XML
    {
        class Document;
        class Element;
        class Node;
        class NodeList;
    }
}


namespace XULWin
{


    class XULRunner;


    namespace Js
    {
        class JsSimpleContext;
        class JsException;

        /**
         * Js::JsXULRunner bundles a XULWin::XULRunner and a Js state.
         * You should use a separate JsXULRunner object for each XUL document
         * that you want to parse. This ensures that a separate Js state will be
         * created per XUL document.
         */
        class JsXULRunner : public XULWin::NativeEventListener
        {
        public:
            JsXULRunner();

            ~JsXULRunner();

            typedef boost::function<void(const JsException &)> JsExceptionLogger;

            void setExceptionLogger(const JsExceptionLogger & inLogger);

            void run(const std::string & inApplicationIniFile);

            Poco::XML::Document * loadApplication(const std::string & inApplicationIniFile);

            Poco::XML::Document * loadXULFromFile(const std::string & inXULUrl);

            Poco::XML::Document * loadXULFromString(const std::string & inXULString);
            
            Poco::XML::Document * document();

            Poco::XML::Element * rootElement() const;

        private:
            virtual LRESULT handleCommand(Component * inSender, WORD inNotificationCode, WPARAM wParam, LPARAM lParam);
            //virtual LRESULT handleMenuCommand(Component * inSender, WORD inMenuId);
            virtual LRESULT handleDialogCommand(Component * inSender, WORD inNotificationCode, WPARAM wParam, LPARAM lParam);
            virtual LRESULT handleMessage(Component * inSender, UINT inMessage, WPARAM wParam, LPARAM lParam);
            
            void initializeJavaScript(Poco::XML::Element * inRootElement);
            
            void logJsException(const JsException & inJsException);

            void loadScripts(Poco::XML::Element * inEl);

            void loadScript(const std::string & inText);

            void addListeners(Poco::XML::Element * inEl);

            boost::scoped_ptr<XULWin::XULRunner> mXULRunner;
            boost::scoped_ptr<JsSimpleContext> mSimpleContext;
            JsExceptionLogger mLogger;
        };


    } // namespace Js

} // namespace XULWin


#endif // XULRUNNERJS_H_INCLUDED
