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

    void initialize();

    void finalize();

    Element * loadApplication(const std::string & inFile);

    void showModal(Element * inWindow);


} // namespace Lua

} // namespace XULWin


#endif // LUABINDINGS_H_INCLUDED
