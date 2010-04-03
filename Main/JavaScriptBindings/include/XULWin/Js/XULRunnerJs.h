#ifndef XULRUNNERWITHLUA_H_INCLUDED
#define XULRUNNERWITHLUA_H_INCLUDED


#include "XULWin/Element.h"
#include "XULWin/EventListener.h"
#include "XULWin/Windows.h"
#include "v8.h"
#include <boost/scoped_ptr.hpp>
#include <string>


namespace XULWin
{


    class XULRunner;


    namespace Js
    {

        /**
         * Js::XULRunnerJs bundles a XULWin::XULRunner and a Js state.
         * You should use a separate XULRunnerJs object for each XUL document
         * that you want to parse. This ensures that a separate Js state will be
         * created per XUL document.
         */
        class XULRunnerJs : public EventListener
        {
        public:
            XULRunnerJs(HMODULE hModuleHandle);

            ~XULRunnerJs();

            ElementPtr loadApplication(const std::string & inApplicationIniFile);

            ElementPtr loadXUL(const std::string & inXULUrl);

            ElementPtr rootElement() const;

            boost::function<void(const std::string &)> Logger;

            bool loadFile(const std::string & inJsFile);

            bool loadScript(const std::string & inScript);

            HMODULE getModuleHandle() const;

        private:

            virtual LRESULT handleCommand(Element * inSender, WORD inNotificationCode, WPARAM wParam, LPARAM lParam);
            virtual LRESULT handleMenuCommand(Element * inSender, WORD inMenuId);
            virtual LRESULT handleDialogCommand(Element * inSender, WORD inNotificationCode, WPARAM wParam, LPARAM lParam);
            virtual LRESULT handleMessage(Element * inSender, UINT inMessage, WPARAM wParam, LPARAM lParam);

            void log(const std::string & inMessage);

            bool executeString(v8::Handle<v8::String> source, v8::Handle<v8::String> name);

            void loadScripts(Element * inEl);

            void addListeners(Element * inEl);

            HMODULE mModuleHandle;
            boost::scoped_ptr<XULWin::XULRunner> mXULRunner;
            v8::HandleScope mHandleScope;
            v8::Handle<v8::Context> mContext;
            v8::Handle<v8::ObjectTemplate> mGlobalObject;
        };


    } // namespace Js

} // namespace XULWin


#endif // XULRUNNERWITHLUA_H_INCLUDED
