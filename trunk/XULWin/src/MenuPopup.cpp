#include "XULWin/MenuPopup.h"
#include "XULWin/MenuPopupComponent.h"
#include "XULWin/AttributeController.h"
#include "XULWin/Decorator.h"


namespace XULWin
{
    
    MenuPopup::MenuPopup(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(MenuPopup::Type(),
                inParent,
                new MenuPopupComponent(inParent->component(), inAttributesMapping))
    {
    }

    
    MenuPopup::~MenuPopup()
    {
    }

} // namespace XULWin
