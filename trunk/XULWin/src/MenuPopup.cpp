#include "XULWin/MenuPopup.h"
#include "XULWin/Decorator.h"
#include "XULWin/Menu.h"
#include "XULWin/MenuItem.h"


namespace XULWin
{
    
    MenuPopupComponent::MenuPopupComponent(Component * inParent, const AttributesMapping & inAttributesMapping) :
        PassiveComponent(inParent, inAttributesMapping)
    {
    }

        
    Windows::PopupMenu * MenuPopupComponent::getMenu()
    {
        Windows::PopupMenu * popupMenu = new Windows::PopupMenu;
        for (size_t idx = 0; idx != getChildCount(); ++idx)
        {
            ElementPtr child = el()->children()[idx];
            if (MenuItemComponent * menuItem = child->component()->downcast<MenuItemComponent>())
            {
                popupMenu->append(new Windows::PopupMenuItem(menuItem->commandId(), menuItem->getLabel()));
            }
            else if (MenuComponent * menu = child->component()->downcast<MenuComponent>())
            {
                if (!menu->el()->children().empty())
                {
                    if (MenuPopupComponent * childPopup = menu->el()->children()[0]->component()->downcast<MenuPopupComponent>())
                    {
                        if (MenuComponent * menu = childPopup->parent()->downcast<MenuComponent>())
                        {
                            popupMenu->append(menu->getLabel(), childPopup->getMenu());
                        }
                    }
                }
            }
        }
        return popupMenu;
    }

        
    void MenuPopupComponent::show(RECT inExcludeRect)
    {
        if (NativeComponent * comp = NativeControl::GetNativeThisOrParent(this))
        {
            POINT location;
            location.x = inExcludeRect.left;
            location.y = inExcludeRect.bottom;
            //::MapWindowPoints(comp->handle(), HWND_DESKTOP, &location, 1);
            boost::scoped_ptr<Windows::PopupMenu> menu(getMenu());
            menu->show(comp->handle(), location, inExcludeRect);
        }
    }
    
    
    void MenuPopupComponent::onChildAdded(Component * inChild)
    {
        parent()->onContentChanged();
    }


    void MenuPopupComponent::onChildRemoved(Component * inChild)
    {
        parent()->onContentChanged();
    }
    
} // namespace XULWin
