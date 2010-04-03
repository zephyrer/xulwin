#include "XULWin/Js/XULRunnerJs.h"
#include "XULWin/ErrorReporter.h"
#include "XULWin/Component.h"
#include "XULWin/Decorator.h"
#include "XULWin/Initializer.h"
#include "XULWin/ScriptElement.h"
#include "XULWin/Windows.h"
#include "XULWin/WinUtils.h"
#include "XULWin/XULRunner.h"
#ifdef _DEBUG
#include "v8-debug.h"
#else
#include "v8.h"
#endif
#include <sstream>
#include <map>


using namespace v8;


namespace XULWin
{


    namespace Js
    {


        XULRunnerJs::XULRunnerJs(HMODULE inModuleHandle) :
            mModuleHandle(inModuleHandle),
            mXULRunner(new XULRunner(inModuleHandle))
        {
        }


        XULRunnerJs::~XULRunnerJs()
        {
        }


        HMODULE XULRunnerJs::getModuleHandle() const
        {
            return mXULRunner->getModuleHandle();
        }


        void XULRunnerJs::log(const std::string & inMessage)
        {
            if (Logger)
            {
                Logger(inMessage);
            }
        }


        bool XULRunnerJs::loadFile(const std::string & inJsFile)
        {
            return false;
        }


        Handle<Value> JSAlert(const Arguments & args)
        {
            if (args.Length() > 0)
            {
                String::AsciiValue ascii0(args[0]);
                MessageBoxA(NULL, *ascii0, "Javascript message", MB_OK);
            }
            return v8::Undefined();
        }


        /**
         * executeString
         *
         * Executes a string within the current v8 context.
         */
        bool XULRunnerJs::executeString(v8::Handle<v8::String> inSource, v8::Handle<v8::String> inName)
        {
            v8::Context::Scope contextScope(mContext);
            v8::TryCatch tryCatch;
            v8::Handle<v8::Script> script = v8::Script::Compile(inSource, inName);
            if (script.IsEmpty())
            {
                v8::String::AsciiValue error(tryCatch.Exception());
                log(*error);
                return false;
            }

            v8::Handle<v8::Value> result = script->Run();
            if (result.IsEmpty())
            {
                v8::String::AsciiValue error(tryCatch.Exception());
                log(*error);
                return false;
            }
            return true;
        }


        bool XULRunnerJs::loadScript(const std::string & inScript)
        {
            //convert the string with the script to a v8 string
            v8::Handle<v8::String> source = v8::String::New(inScript.c_str());

            //each script name must be unique , for this demo I just run one embedded script, so the inName can be fixed
            v8::Handle<v8::String> name = v8::String::New("Test message");

            // Create a template for the globl object.
            mGlobalObject = v8::ObjectTemplate::New();

            //associates "plus" on script to the Plus function
            mGlobalObject->Set(v8::String::New("alert"), v8::FunctionTemplate::New(XULWin::Js::JSAlert));

            //create context for the script
            mContext = v8::Context::New(NULL, mGlobalObject);

            //simple javascritp to test
            executeString(source, name);
            return true;
        }


        ElementPtr XULRunnerJs::loadApplication(const std::string & inApplicationIniFile)
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


        ElementPtr XULRunnerJs::loadXUL(const std::string & inXULUrl)
        {
            ElementPtr result = mXULRunner->loadXUL(inXULUrl);
            if (result)
            {
                loadScripts(result.get());
                addListeners(result.get());
            }
            return result;
        }


        ElementPtr XULRunnerJs::rootElement() const
        {
            return mXULRunner->rootElement();
        }


        void XULRunnerJs::loadScripts(Element * inElement)
        {
            std::vector<XULWin::ScriptElement *> scripts;
            inElement->getElementsByType<ScriptElement>(scripts);
            for (size_t idx = 0; idx != scripts.size(); ++idx)
            {
                loadScript(scripts[idx]->innerText());
            }
        }


        void XULRunnerJs::addListeners(Element * inElement)
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


        LRESULT XULRunnerJs::handleCommand(Element * inSender, WORD inNotificationCode, WPARAM wParam, LPARAM lParam)
        {
            std::string oncommand = inSender->getAttribute("oncommand");
            if (!oncommand.empty())
            {
                loadScript(oncommand);
                return 0;
            }
            return 1;
        }


        LRESULT XULRunnerJs::handleMenuCommand(Element * inSender, WORD inMenuId)
        {
            return 1;
        }


        LRESULT XULRunnerJs::handleDialogCommand(Element * inSender, WORD inNotificationCode, WPARAM wParam, LPARAM lParam)
        {
            return 1;
        }


        LRESULT XULRunnerJs::handleMessage(Element * inSender, UINT inMessage, WPARAM wParam, LPARAM lParam)
        {
            return 1;
        }


    } // namespace Js

} // namespace XULWin
