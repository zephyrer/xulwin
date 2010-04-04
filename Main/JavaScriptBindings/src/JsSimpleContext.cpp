#include "XULWin/Js/JsSimpleContext.h"
#include "XULWin/Js/JsUtilities.h"
#ifdef _DEBUG
#include "v8-debug.h"
#else
#include "v8.h"
#endif


using namespace v8;


namespace XULWin
{


    namespace Js
    {
        
        POCO_IMPLEMENT_EXCEPTION(JsException, Poco::RuntimeException, "JavaScript Exception")


        JsSimpleContext::JsSimpleContext()
        {
            // Create a template for the globl object.
            mGlobalObject = v8::ObjectTemplate::New();

            //associates "plus" on script to the Plus function
            mGlobalObject->Set(v8::String::New("alert"), v8::FunctionTemplate::New(XULWin::Js::Alert));

            //create context for the script
            mContext = v8::Context::New(NULL, mGlobalObject);
        }


        JsSimpleContext::~JsSimpleContext()
        {
        }


        /**
         * executeString
         *
         * Executes a script within the current v8 context.
         */
        bool JsSimpleContext::executeString(v8::Handle<v8::String> inSource, v8::Handle<v8::String> inName)
        {
            v8::Context::Scope contextScope(mContext);
            v8::TryCatch tryCatch;
            v8::Handle<v8::Script> script = v8::Script::Compile(inSource, inName);
            if (script.IsEmpty())
            {
                v8::String::AsciiValue error(tryCatch.Exception());
                throw std::runtime_error(*error);
            }

            v8::Handle<v8::Value> result = script->Run();
            if (result.IsEmpty())
            {
                v8::String::AsciiValue error(tryCatch.Exception());
                throw std::runtime_error(*error);
            }
            return true;
        }


        bool JsSimpleContext::loadScript(const std::string & inScript)
        {
            //convert the string with the script to a v8 string
            v8::Handle<v8::String> source = v8::String::New(inScript.c_str());

            //each script name must be unique , for this demo I just run one embedded script, so the inName can be fixed
            v8::Handle<v8::String> name = v8::String::New("Test message");

            //simple javascritp to test
            executeString(source, name);
            return true;
        }


    } // namespace Js

} // namespace XULWin
