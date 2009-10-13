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
#ifndef SWIG
    class XULRunnerWithLua;
    XULRunnerWithLua * setXULRunner(XULRunnerWithLua * inXULRunner);
#endif // SWIG

    // shows a mesage box
    void showMessage(const std::string & inString);

    // same to DOM's 'document' object
    Element * getRootElement();

    // behaves the same as Javascript's setTimeout function
    void setTimeout(const std::string & inCallback, int inMilliseconds);

    // behaves the sames as Javascript's prompt function
    std::string prompt(const std::string & inText, const std::string & inDefault);

    // converts Element to Window, returns 0 on fail
    Window * toWindow(Element * inElement);


} // namespace Lua

} // namespace XULWin


#endif // LUABINDINGS_H_INCLUDED
