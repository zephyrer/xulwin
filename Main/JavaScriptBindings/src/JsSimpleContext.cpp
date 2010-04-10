#include "XULWin/Js/JsSimpleContext.h"
#include "XULWin/Js/JsUtilities.h"


using namespace v8;


namespace XULWin
{


    namespace Js
    {

        std::stack<JsSimpleContext*> JsSimpleContext::sInstances;

        JsSimpleContext::JsSimpleContext(Element * inRootElement) :
            mRootElement(inRootElement)
        {
            sInstances.push(this);
            mGlobalObject = v8::ObjectTemplate::New();
            if (mGlobalObject.IsEmpty())
            {
                throw std::runtime_error("Failed to create the global JavaScript object.");
            }
            
            registerFunction("alert", Js::Alert);
            registerFunction("prompt", Js::Prompt);

            mContext = v8::Context::New(NULL, mGlobalObject);
            if (mContext.IsEmpty())
            {
                throw std::runtime_error("Failed to create a context for the global JavaScript object.");
            }
        }


        JsSimpleContext::~JsSimpleContext()
        {
            sInstances.pop();
        }


        const std::stack<JsSimpleContext*> & JsSimpleContext::Instances()
        {
            return sInstances;
        }

        
        void JsSimpleContext::registerFunction(const std::string & inName, v8::InvocationCallback inCallback)
        {
            mGlobalObject->Set(v8::String::New(inName.c_str()), v8::FunctionTemplate::New(inCallback));
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


        Element * JsSimpleContext::rootElement() const
        {
            return mRootElement;
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
