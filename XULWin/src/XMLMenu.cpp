#include "XULWin/XMLMenu.h"
#include "XULWin/Menu.h"
#include "XULWin/Decorator.h"


namespace XULWin
{

    XMLMenu::XMLMenu(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(XMLMenu::TagName(),
                inParent,
                new Menu(inParent->component(), inAttributesMapping))
    {
    }

} // namespace XULWin
