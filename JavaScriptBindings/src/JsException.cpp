#include "XULWin/Js/JsException.h"


namespace XULWin
{

    namespace Js
    {

        POCO_IMPLEMENT_EXCEPTION(JsException, Poco::RuntimeException, "JavaScript Exception")

    } // namespace Js

} // namespace XULWin
