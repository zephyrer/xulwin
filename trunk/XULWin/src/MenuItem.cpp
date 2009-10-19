#include "XULWin/MenuItem.h"
#include "XULWin/MenuItemComponent.h"
#include "XULWin/AttributeController.h"
#include "XULWin/Decorator.h"


namespace XULWin
{
    
    MenuItem::MenuItem(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(MenuItem::Type(),
                inParent,
                new MenuItemComponent(inParent->component(), inAttributesMapping))
    {
    }
        
    
    MenuItem::~MenuItem()
    {
    }

    
    std::string MenuItem::label() const
    {
        return getAttribute("label");
    }

    
    std::string MenuItem::value() const
    {
        return getAttribute("value");
    }

} // namespace XULWin
