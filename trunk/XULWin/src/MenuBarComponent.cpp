#include "XULWin/MenuBarComponent.h"
#include "XULWin/Decorator.h"
#include "XULWin/Defaults.h"
#include "XULWin/MenuItem.h"
#include "XULWin/MenuComponent.h"
#include "XULWin/MenuItemComponent.h"


namespace XULWin
{
    
    MenuBarComponent::MenuBarComponent(Component * inParent, const AttributesMapping & inAttributesMapping) :
        PassiveComponent(inParent, inAttributesMapping)
    {
    }
    

    int MenuBarComponent::calculateWidth(SizeConstraint inSizeConstraint) const
    {
        int result = 0;
        std::vector<Element*> items;
        el()->getElementsByType(MenuItem::Type(), items);
        for (size_t idx = 0; idx != items.size(); ++idx)
        {
            MenuItemComponent * item = items[idx]->component()->downcast<MenuItemComponent>();
            result += item->calculateWidth(inSizeConstraint) + Defaults::menuBarSpacing();
        }     
        return result;
    }


    int MenuBarComponent::calculateHeight(SizeConstraint inSizeConstraint) const
    {
        return 0; // does not take part in client rect
        //return Defaults::menuBarHeight();
    }
    
} // namespace XULWin
