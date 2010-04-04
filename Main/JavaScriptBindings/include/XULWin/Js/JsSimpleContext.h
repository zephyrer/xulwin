#ifndef SIMPLEJSCONTEXT_H_INCLUDED
#define SIMPLEJSCONTEXT_H_INCLUDED


#include "XULWin/Js/V8Includes.h"
#include "Poco/Exception.h"


#ifndef XULWIN_API
#define XULWIN_API
#endif // XULWIN_API


namespace XULWin
{


    namespace Js
    {

        // Declare the JsException class. It inherits Poco::RuntimeException.
        POCO_DECLARE_EXCEPTION(XULWIN_API, JsException, Poco::RuntimeException)


        /**
         * JsSimpleContext
         *
         * This class simplifies the usage of the V8 JavaScript library.
         */
        class JsSimpleContext
        {
        public:
            JsSimpleContext();

            ~JsSimpleContext();
            
            bool loadScript(const std::string & inScript);

        private:
            bool executeString(v8::Handle<v8::String> inScriptSource, v8::Handle<v8::String> inScriptName);

            v8::HandleScope mHandleScope;
            v8::Handle<v8::Context> mContext;
            v8::Handle<v8::ObjectTemplate> mGlobalObject;
        };


    } // namespace Js

} // namespace XULWin


#endif // SIMPLEJSCONTEXT_H_INCLUDED
