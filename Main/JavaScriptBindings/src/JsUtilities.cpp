#include "XULWin/Js/JsUtilities.h"
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

} // namespace Js

} // namespace XULWin
