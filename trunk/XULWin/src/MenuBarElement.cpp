#include "XULWin/MenuBarElement.h"
#include "XULWin/MenuBar.h"
#include "XULWin/AttributeController.h"
#include "XULWin/Decorator.h"


namespace XULWin
{

    MenuBarElement::MenuBarElement(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(MenuBarElement::Type(),
                inParent,
                new MenuBarComponent(inParent->component(), inAttributesMapping))
    {
    }

} // namespace XULWin
