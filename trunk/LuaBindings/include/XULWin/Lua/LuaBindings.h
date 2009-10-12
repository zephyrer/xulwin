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

    void showMessage(const std::string & inString);

    // returns the previous root element
    Element * setRootElement(Element * inEl);

    Element * getRootElement();

    std::string prompt(const std::string & inText, const std::string & inDefault);

    Window * toWindow(Element * inElement);


} // namespace Lua

} // namespace XULWin


#endif // LUABINDINGS_H_INCLUDED
