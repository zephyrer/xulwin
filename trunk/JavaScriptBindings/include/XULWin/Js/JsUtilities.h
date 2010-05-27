#ifndef JSUTILITIES_H_INCLUDED
#define JSUTILITIES_H_INCLUDED


#include "XULWin/Js/V8Includes.h"
#include "XULWin/WindowElement.h"


namespace XULWin
{

    namespace Js
    {

        /**
         * Alert
         *
         * Implements the JavaScript 'window.alert' function.
         */
        v8::Handle<v8::Value> Alert(const v8::Arguments & args);


        /**
         * Prompt
         *
         * Implements the JavaScript 'window.prompt' function.
         */
        v8::Handle<v8::Value> Prompt(const v8::Arguments & args);


    } // namespace Js

} // namespace XULWin


#endif // JSUTILITIES_H_INCLUDED
