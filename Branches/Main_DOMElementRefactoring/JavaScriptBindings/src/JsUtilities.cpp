#include "XULWin/Js/JsUtilities.h"
#include "XULWin/ComponentManager.h"
#include "XULWin/Js/JsPrompt.h"
#include "XULWin/Js/JsSimpleContext.h"
#include "Windows.h"


using namespace v8;


namespace XULWin
{

namespace Js
{

    Handle<Value> Alert(const Arguments & args)
    {
        if (args.Length() > 0)
        {
            String::AsciiValue ascii0(args[0]);
            ::MessageBoxA(NULL, *ascii0, "Javascript message", MB_OK);
        }
        return v8::Undefined();
    }


    Handle<Value> Prompt(const Arguments & args)
    {
        if (JsSimpleContext::Instances().empty())
        {
            throw std::runtime_error("Prompt was called without a JsSimpleContext instance.");
        }

        int argc = args.Length();
        if (argc < 1)
        {
            return v8::Undefined();
        }

        const JsSimpleContext * currentContext = JsSimpleContext::Instances().top();
        Window * windowElement = 0;
        if (currentContext->rootElement())
        {
            windowElement = GetComponent<Window>(currentContext->rootElement());
        }

        std::string arg0, arg1;

        if (argc >= 1)
        {
            String::AsciiValue ascii0(args[0]);
            arg0 = *ascii0;
        }

        if (argc >= 2)
        {
            String::AsciiValue ascii1(args[1]);
            arg1 = *ascii1;
        }


        JsPrompt prompt(windowElement);
        Fallible<std::string> result = prompt.show(arg0, arg1);
        if (result.isValid())
        {
            return v8::String::New(result.getValue().c_str());
        }
        else
        {
            return v8::Null();
        }
    }

} // namespace Js

} // namespace XULWin
