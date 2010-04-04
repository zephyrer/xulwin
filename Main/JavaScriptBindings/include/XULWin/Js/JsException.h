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
         * Propagation of JavaScript exceptions into C++ land.         
         * 
         * JsExeption inherits Poco::RuntimeException.
         * The implementation is defined as a macro in JsSimpleContext.cpp. 
         */
        POCO_DECLARE_EXCEPTION(XULWIN_API, JsException, Poco::RuntimeException)

    } // namespace Js

} // namespace XULWin


#endif // JSEXCEPTION_H_INCLUDED
