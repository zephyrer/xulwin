#include "XULWin/MenuPopup.h"
#include "XULWin/Decorator.h"
#include "XULWin/Menu.h"
#include "XULWin/MenuItem.h"


namespace XULWin
{
    
    MenuPopup::MenuPopup(Component * inParent, const AttributesMapping & inAttributesMapping) :
        PassiveComponent(inParent, inAttributesMapping)
    {
    }

        
    Windows::PopupMenu * MenuPopup::getMenu()
    {
        Windows::PopupMenu * popupMenu = new Windows::PopupMenu;
        for (size_t idx = 0; idx != getChildCount(); ++idx)
        {
            ElementPtr child = el()->children()[idx];
            if (MenuItem * menuItem = child->component()->downcast<MenuItem>())
            {
                popupMenu->append(new Windows::PopupMenuItem(menuItem->commandId(), menuItem->getLabel()));
            }
            else if (Menu * menu = child->component()->downcast<Menu>())
            {
                if (!menu->el()->children().empty())
                {
                    if (MenuPopup * childPopup = menu->el()->children()[0]->component()->downcast<MenuPopup>())
                    {
                        if (Menu * menu = childPopup->parent()->downcast<Menu>())
                        {
                            popupMenu->append(menu->getLabel(), childPopup->getMenu());
                        }
                    }
                }
            }
        }
        return popupMenu;
    }

        
    void MenuPopup::show(RECT inExcludeRect)
    {
        if (NativeComponent * comp = NativeControl::GetThisOrParent(this))
        {
            POINT location;
            location.x = inExcludeRect.left;
            location.y = inExcludeRect.bottom;
            //::MapWindowPoints(comp->handle(), HWND_DESKTOP, &location, 1);
            boost::scoped_ptr<Windows::PopupMenu> menu(getMenu());
            menu->show(comp->handle(), location, inExcludeRect);
        }
    }
    
    
    void MenuPopup::onChildAdded(Component * inChild)
    {
        parent()->onContentChanged();
    }


    void MenuPopup::onChildRemoved(Component * inChild)
    {
        parent()->onContentChanged();
    }
    
} // namespace XULWin
