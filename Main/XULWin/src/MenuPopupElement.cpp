#include "XULWin/MenuPopupElement.h"
#include "XULWin/MenuPopup.h"
#include "XULWin/AttributeController.h"
#include "XULWin/Decorator.h"


namespace XULWin
{

    MenuPopupElement::MenuPopupElement(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(MenuPopupElement::TagName(),
                inParent,
                new MenuPopup(inParent->component(), inAttributesMapping))
    {
    }


    MenuPopupElement::~MenuPopupElement()
    {
    }

} // namespace XULWin
