#include "XULWin/MenuPopupElement.h"
#include "XULWin/MenuPopup.h"
#include "XULWin/AttributeController.h"
#include "XULWin/Decorator.h"


namespace XULWin
{
    
    MenuPopupElement::MenuPopupElement(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(MenuPopupElement::Type(),
                inParent,
                new MenuPopupComponent(inParent->component(), inAttributesMapping))
    {
    }

    
    MenuPopupElement::~MenuPopupElement()
    {
    }

} // namespace XULWin
