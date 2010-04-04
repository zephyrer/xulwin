#ifndef SIMPLEJSCONTEXT_H_INCLUDED
#define SIMPLEJSCONTEXT_H_INCLUDED


#include "XULWin/Js/V8Includes.h"
#include "XULWin/Js/JsException.h"
#include "Poco/Exception.h"


namespace XULWin
{


    namespace Js
    {

        /**
         * JsSimpleContext
         *
         * This class simplifies the usage of the V8 JavaScript library.
         * Each object defines a new JavaScript context.
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
