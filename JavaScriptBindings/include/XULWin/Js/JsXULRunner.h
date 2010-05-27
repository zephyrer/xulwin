#ifndef XULRUNNERJS_H_INCLUDED
#define XULRUNNERJS_H_INCLUDED


#include "XULWin/Element.h"
#include "XULWin/EventListener.h"
#include "XULWin/Windows.h"
#include <boost/function.hpp>
#include <boost/scoped_ptr.hpp>
#include <string>


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
        class JsXULRunner : public EventListener
        {
        public:
            JsXULRunner(HMODULE hModuleHandle);

            ~JsXULRunner();

            typedef boost::function<void(const JsException &)> JsExceptionLogger;

            void setExceptionLogger(const JsExceptionLogger & inLogger);

            void run(const std::string & inApplicationIniFile);

            ElementPtr loadApplication(const std::string & inApplicationIniFile);

            ElementPtr loadXULFromFile(const std::string & inXULUrl);

            ElementPtr loadXULFromString(const std::string & inXULString);

            ElementPtr rootElement() const;

            HMODULE getModuleHandle() const;

        private:
            virtual LRESULT handleCommand(Element * inSender, WORD inNotificationCode, WPARAM wParam, LPARAM lParam);
            virtual LRESULT handleMenuCommand(Element * inSender, WORD inMenuId);
            virtual LRESULT handleDialogCommand(Element * inSender, WORD inNotificationCode, WPARAM wParam, LPARAM lParam);
            virtual LRESULT handleMessage(Element * inSender, UINT inMessage, WPARAM wParam, LPARAM lParam);
            
            void initializeJavaScript(Element * inRootElement);
            
            void logJsException(const JsException & inJsException);

            void loadScripts(Element * inEl);

            void addListeners(Element * inEl);

            HMODULE mModuleHandle;
            boost::scoped_ptr<XULWin::XULRunner> mXULRunner;
            boost::scoped_ptr<JsSimpleContext> mSimpleContext;
            JsExceptionLogger mLogger;
        };


    } // namespace Js

} // namespace XULWin


#endif // XULRUNNERJS_H_INCLUDED
