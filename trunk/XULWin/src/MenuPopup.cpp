#include "XULWin/MenuPopup.h"
#include "XULWin/MarginDecorator.h"
#include "XULWin/Element.h"
#include "XULWin/Menu.h"
#include "XULWin/MenuItem.h"


namespace XULWin
{
    
    Component * CreateMenuPopup(Component * inComponent, const AttributesMapping & inAttr)
    {
        return new MarginDecorator(new MenuPopup(inComponent, inAttr));
    }


    MenuPopup::MenuPopup(Component * inParent, const AttributesMapping & inAttr) :
        PhonyComponent(inParent, inAttr)
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
                popupMenu->append(new Windows::PopupMenuItem(menuItem->componentId(), menuItem->getLabel()));
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


    int MenuPopup::calculateWidth(SizeConstraint inSizeConstraint) const
    {
        /**
         * Popup-menus don't *need* any width in the layouting of the window,
         * but it may assert its width in order to have the widgets that are aligned
         * width the popup menu to have the same width as the menu itself.
         * NOTE: this needs revision (since there will be situations where this behavior
         *       is absolutely not desired.
         */
        return calculateMaxChildWidth(inSizeConstraint);
    }


    int MenuPopup::calculateHeight(SizeConstraint inSizeConstraint) const
    {
        /**
         * Popup menu don't ask for height in the layouting of the window.
         */
        return 0;
    }


    void MenuPopup::show(RECT inExcludeRect)
    {
        if (NativeComponent * comp = NativeControl::FindNativeParent(this))
        {
            POINT location;
            location.x = inExcludeRect.left;
            location.y = inExcludeRect.bottom;
            //::MapWindowPoints(comp->handle(), HWND_DESKTOP, &location, 1);
            boost::scoped_ptr<Windows::PopupMenu> menu(getMenu());
            menu->show(location, inExcludeRect);
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
