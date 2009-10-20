#include "XULWin/MenuBar.h"
#include "XULWin/Decorator.h"
#include "XULWin/Defaults.h"
#include "XULWin/MenuItemElement.h"
#include "XULWin/Menu.h"
#include "XULWin/MenuItem.h"


namespace XULWin
{
    
    MenuBar::MenuBar(Component * inParent, const AttributesMapping & inAttributesMapping) :
        PassiveComponent(inParent, inAttributesMapping)
    {
    }
    

    int MenuBar::calculateWidth(SizeConstraint inSizeConstraint) const
    {
        int result = 0;
        std::vector<Element*> items;
        el()->getElementsByType(MenuItemElement::Type(), items);
        for (size_t idx = 0; idx != items.size(); ++idx)
        {
            MenuItem * item = items[idx]->component()->downcast<MenuItem>();
            result += item->calculateWidth(inSizeConstraint) + Defaults::menuBarSpacing();
        }     
        return result;
    }


    int MenuBar::calculateHeight(SizeConstraint inSizeConstraint) const
    {
        return 0; // does not take part in client rect
        //return Defaults::menuBarHeight();
    }
    
} // namespace XULWin
