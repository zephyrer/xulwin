#ifndef JSEXCEPTION_H_INCLUDED
#define JSEXCEPTION_H_INCLUDED


#include "Poco/Exception.h"


#ifndef XULWIN_API
#define XULWIN_API
#endif // XULWIN_API


namespace XULWin
{

    namespace Js
    {

        /**
         * JsException
         *
         * Propagates JavaScript exceptions into C++ land.
         * Inherits from Poco::RuntimeException.
         */
        POCO_DECLARE_EXCEPTION(XULWIN_API, JsException, Poco::RuntimeException)

    } // namespace Js

} // namespace XULWin


#endif // JSEXCEPTION_H_INCLUDED
