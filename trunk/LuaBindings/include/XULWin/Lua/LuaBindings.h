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

    void showMessage(const std::string & inString);

    Element * getRootElement();

    void setTimeout(const std::string & inCallback, int inMilliseconds);

    std::string prompt(const std::string & inText, const std::string & inDefault);

    Window * toWindow(Element * inElement);


} // namespace Lua

} // namespace XULWin


#endif // LUABINDINGS_H_INCLUDED
