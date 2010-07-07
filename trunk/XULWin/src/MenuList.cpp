#include "XULWin/MenuList.h"
#include "XULWin/Decorators.h"
#include "XULWin/Defaults.h"
#include "XULWin/Element.h"
#include "XULWin/MenuItem.h"
#include "XULWin/MenuPopup.h"
#include "XULWin/WinUtils.h"
 

namespace XULWin
{

    Component * CreateMenuList(Component * inComponent, const AttributesMapping & inAttr)
    {
        return new MarginDecorator(new MenuList(inComponent, inAttr));
    }


    MenuList::MenuList(Component * inParent, const AttributesMapping & inAttr) :
        NativeControl(
            inParent,
            inAttr,
            TEXT("COMBOBOX"),
            0, // exStyle
            WS_TABSTOP | CBS_DROPDOWNLIST)
    {
    }


    bool MenuList::init()
    {
        fillComboBox();
        return Super::init();
    }


    void MenuList::fillComboBox()
    {
        if (MenuPopup * popup = findChildOfType<MenuPopup>())
        {
            for (size_t idx = 0; idx != popup->getChildCount(); ++idx)
            {
                ElementPtr child = popup->el()->children()[idx];
                if (MenuItem * item = child->component()->downcast<MenuItem>())
                {
                    std::string label = item->getLabel();
                    WinAPI::addStringToComboBox(handle(), label);
                }
            }
            WinAPI::selectComboBoxItem(handle(), 0);
        }
    }


    int MenuList::calculateWidth(SizeConstraint inSizeConstraint) const
    {
        return Defaults::menuListMinWidth() + calculateMaxChildWidth(inSizeConstraint);
    }


    int MenuList::calculateHeight(SizeConstraint inSizeConstraint) const
    {
        return Defaults::controlHeight();
    }


    void MenuList::move(int x, int y, int w, int h)
    {
        // The height of a combobox in Win32 is the height of the dropdown menu
        // + the height of the widget itself.

        int numItems = WinAPI::getComboBoxItemCount(handle());
        int dropdownHeight = 0;
        if (numItems > 0)
        {
            dropdownHeight = numItems * WinAPI::getComboBoxItemHeight(handle(), 0); // use index 0
        }

        // This is usually needed as well, I think :S
        int extraHeight = WinAPI::getSizeDifferenceBetweenWindowRectAndClientRect(handle()).cy;

        NativeControl::move(x, y, w, h + dropdownHeight + extraHeight);
    }


    void MenuList::onContentChanged()
    {
        if (mIsInitialized)
        {
            WinAPI::clearComboBox(handle());
            fillComboBox();
        }
        // else: the init will take care of the initial fill
    }

} // namespace XULWin
