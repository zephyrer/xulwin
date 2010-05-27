#ifndef SIMPLEJSCONTEXT_H_INCLUDED
#define SIMPLEJSCONTEXT_H_INCLUDED


#include "XULWin/WindowElement.h"
#include "XULWin/Js/V8Includes.h"
#include "XULWin/Js/JsException.h"
#include "Poco/Exception.h"
#include <stack>


namespace XULWin
{


    namespace Js
    {

        /**
         * JsSimpleContext
         *
         * This class simplifies the usage of the V8 JavaScript library.
         * Associates a JavaScript context with the root element of a XUL document.
         */
        class JsSimpleContext
        {
        public:
            JsSimpleContext(Element * inRootElement);

            ~JsSimpleContext();

            static const std::stack<JsSimpleContext*> & Instances();

            Element * rootElement() const;
            
            bool loadScript(const std::string & inScript);

        private:
            void registerFunction(const std::string & inName, v8::InvocationCallback inCallback);

            bool executeString(v8::Handle<v8::String> inScriptSource, v8::Handle<v8::String> inScriptName);

            v8::HandleScope mHandleScope;
            v8::Handle<v8::Context> mContext;
            v8::Handle<v8::ObjectTemplate> mGlobalObject;

            static std::stack<JsSimpleContext*> sInstances;

            Element * mRootElement;
        };


    } // namespace Js

} // namespace XULWin


#endif // SIMPLEJSCONTEXT_H_INCLUDED
