#include "XULWin/MenuSeparatorElement.h"
#include "XULWin/MenuSeparator.h"
#include "XULWin/AttributeController.h"
#include "XULWin/Decorator.h"


namespace XULWin
{

    MenuSeparatorElement::MenuSeparatorElement(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(MenuSeparatorElement::Type(),
                inParent,
                new MenuSeparator(inParent->component(), inAttributesMapping))
    {
    }

} // namespace XULWin
