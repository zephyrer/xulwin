#include "XULWin/XMLMenu.h"
#include "XULWin/Menu.h"
#include "XULWin/Decorator.h"


namespace XULWin
{

    XMLMenu::XMLMenu(Element * inParent, const AttributesMapping & inAttr) :
        Element(XMLMenu::TagName(),
                inParent,
                new Menu(inParent->component(), inAttr))
    {
    }

} // namespace XULWin
