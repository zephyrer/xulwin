#ifndef LUABINDINGS_H_INCLUDED
#define LUABINDINGS_H_INCLUDED


#include "XULWin/Element.h"
#include "XULWin/Decorator.h"
#include "XULWin/ElementImpl.h"
#include <string>


namespace XULWin
{

namespace Lua
{

    /**
     * Displays a message box.
     */
    void showMessage(const std::string & inString);


    /**
     * Gets the root element for the XUL document.
     * This is the DOM 'document' object.
     */
    Element * getRootElement();

    
    /**
     * Executes the given code after the given delay.
     * Same as Javascript's setTimeout function.
     */
    void setTimeout(const std::string & inExecutableCode, int inMilliseconds);

    
    /**
     * Displays a dialog with a message, input field and OK and Cancel buttons.
     * If the user clicks the Ok button then the user input is returned.
     * If the users clicks cancel then the default value is returned.
     */
    std::string prompt(const std::string & inText, const std::string & inDefault);

    
    /**
     * Casts an Element object to a Window object
     */
    Window * toWindow(Element * inElement);


    /**
     * Casts an Element object to a Dialog object
     */
    Dialog * toDialog(Element * inElement);


    /**
     * PRIVATE FUNCTION
     * Sets the current XULRunnerWithLua object.
     * Returns the old XULRunnerWithLua object.
     */
    #ifndef SWIG
    class XULRunnerWithLua;
    XULRunnerWithLua * setXULRunner(XULRunnerWithLua * inXULRunner);
    #endif


} // namespace Lua

} // namespace XULWin


#endif // LUABINDINGS_H_INCLUDED
