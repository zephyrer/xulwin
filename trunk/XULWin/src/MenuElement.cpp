#include "XULWin/MenuElement.h"
#include "XULWin/Menu.h"
#include "XULWin/AttributeController.h"
#include "XULWin/Decorator.h"


namespace XULWin
{

    MenuElement::MenuElement(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(MenuElement::Type(),
                inParent,
                new MenuComponent(inParent->component(), inAttributesMapping))
    {
    }

} // namespace XULWin
