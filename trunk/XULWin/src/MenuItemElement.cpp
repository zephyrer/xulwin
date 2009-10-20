#include "XULWin/MenuItemElement.h"
#include "XULWin/MenuItem.h"
#include "XULWin/AttributeController.h"
#include "XULWin/Decorator.h"


namespace XULWin
{
    
    MenuItemElement::MenuItemElement(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(MenuItemElement::Type(),
                inParent,
                new MenuItem(inParent->component(), inAttributesMapping))
    {
    }
        
    
    MenuItemElement::~MenuItemElement()
    {
    }

    
    std::string MenuItemElement::label() const
    {
        return getAttribute("label");
    }

    
    std::string MenuItemElement::value() const
    {
        return getAttribute("value");
    }

} // namespace XULWin
