#include "XULWin/Element.h"
#include "XULWin/Defaults.h"
#include "XULWin/Decorator.h"
#include "XULWin/Decorators.h"
#include "XULWin/Menu.h"
#include "XULWin/PopupMenu.h"
#include "XULWin/Window.h"


namespace XULWin
{

    Component * CreateMenu(XULWin::Component * inParent, const AttributesMapping & inAttr)
    {
        return new Decorator(new Menu(inParent, inAttr));
    }


    Menu::MenusById Menu::sMenusById;


    Menu::Menu(Component * inParent, const AttributesMapping & inAttr) :
        PhonyComponent(inParent, inAttr)
    {
        assert(sMenusById.find(mComponentId.value()) == sMenusById.end());
        sMenusById.insert(std::make_pair(mComponentId.value(), this));
    }


    Menu::~Menu()
    {
        MenusById::iterator itById = sMenusById.find(mComponentId.value());
        assert(itById != sMenusById.end());
        if (itById != sMenusById.end())
        {
            sMenusById.erase(itById);
        }
    }


    bool Menu::init()
    {
        return Super::init();
    }


    WinAPI::MenuNode * Menu::FromMenu(const Menu * inMenu)
    {
        // XUL hierarchy:
        // menu/menupopup/menuitem
        //               /menu
        WinAPI::MenuNode * result = new WinAPI::MenuNode(
            WinAPI::MenuItemInfo(inMenu->componentId(),
                                  inMenu->getLabel()));
        const MenuPopup * popup = inMenu->findChildOfType<MenuPopup>();
        for (size_t idx = 0; idx != popup->getChildCount(); ++idx)
        {
            const Component * comp = popup->getChild(idx);
            if (const MenuItem * menuItem = comp->downcast<MenuItem>())
            {
                result->addChild(
                    new WinAPI::MenuNode(
                        WinAPI::MenuItemInfo(menuItem->componentId(),
                                              menuItem->getLabel())));
            }
            else if (const Menu * menu = comp->downcast<Menu>())
            {
                result->addChild(FromMenu(menu));
            }
            else if (const MenuSeparator * sep = comp->downcast<MenuSeparator>())
            {
                result->addChild(new WinAPI::MenuNode(WinAPI::MenuItemInfo(0, "")));
            }
        }
        return result;
    }


    Menu * Menu::FindById(int inId)
    {
        MenusById::iterator itById = sMenusById.find(inId);
        if (itById != sMenusById.end())
        {
            return itById->second;
        }
        return 0;
    }


    bool Menu::initAttributeControllers()
    {
        setAttributeController<LabelController>(this);
        return Super::initAttributeControllers();
    }


    std::string Menu::getLabel() const
    {
        return mLabel;
    }


    void Menu::setLabel(const std::string & inLabel)
    {
        mLabel = inLabel;
    }

    Component * CreateMenuBar(XULWin::Component * inParent, const AttributesMapping & inAttr)
    {
        return new Decorator(new MenuBar(inParent, inAttr));
    }


    MenuBar::MenuBar(Component * inParent, const AttributesMapping & inAttr) :
        PhonyComponent(inParent, inAttr)
    {
    }


    bool MenuBar::init()
    {
        std::vector<XMLMenu *> menuElements;
        el()->getElementsByType<XMLMenu>(menuElements);
        WinAPI::MenuNode node(WinAPI::MenuItemInfo(componentId(), ""));
        for (size_t idx = 0; idx != menuElements.size(); ++idx)
        {
            Menu * menu = menuElements[idx]->component()->downcast<Menu>();
            node.addChild(Menu::FromMenu(menu));
        }
        if (Window * window = findParentOfType<Window>())
        {
            ::SetMenu(window->handle(), WinAPI::createMenu(node));
        }
        return Super::init();
    }



    int MenuBar::calculateWidth(SizeConstraint inSizeConstraint) const
    {
        int result = 0;
        std::vector<Element *> items;
        el()->getElementsByTagName(XMLMenuItem::TagName(), items);
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

    
    Component * CreateMenuItem(Component * inComponent, const AttributesMapping & inAttr)
    {
        return new Decorator(new MenuItem(inComponent, inAttr));
    }


    MenuItem::MenuItemsById MenuItem::sMenuItemsById;


    MenuItem::MenuItem(Component * inParent, const AttributesMapping & inAttr) :
        VirtualComponent(inParent, inAttr)
    {
        assert(sMenuItemsById.find(mComponentId.value()) == sMenuItemsById.end());
        sMenuItemsById.insert(std::make_pair(mComponentId.value(), this));
    }


    MenuItem::~MenuItem()
    {
        MenuItemsById::iterator itById = sMenuItemsById.find(mComponentId.value());
        assert(itById != sMenuItemsById.end());
        if (itById != sMenuItemsById.end())
        {
            sMenuItemsById.erase(itById);
        }
    }


    MenuItem * MenuItem::FindById(int inId)
    {
        MenuItemsById::iterator itById = sMenuItemsById.find(inId);
        if (itById != sMenuItemsById.end())
        {
            return itById->second;
        }
        return 0;
    }


    bool MenuItem::initAttributeControllers()
    {
        setAttributeController<LabelController>(this);
        return Super::initAttributeControllers();
    }


    int MenuItem::calculateWidth(SizeConstraint inSizeConstraint) const
    {
        //int result = 0;
        //if (NativeComponent * comp = NativeControl::FindNativeParent(const_cast<MenuItem *>(this)))
        //{
        //    result = WinAPI::getTextSize(comp->handle(), getLabel()).cx;
        //}
        //return result;

        // FIXME! 
        return 80;
    }


    int MenuItem::calculateHeight(SizeConstraint inSizeConstraint) const
    {
        //if (NativeComponent * comp = NativeControl::FindNativeParent(const_cast<MenuItem *>(this)))
        //{
        //    return WinAPI::getTextSize(comp->handle(), getLabel()).cy;
        //}

        // FIXME!!
        return 80;
    }


    std::string MenuItem::getLabel() const
    {
        return mLabel;
    }


    void MenuItem::setLabel(const std::string & inLabel)
    {
        mLabel = inLabel;
    }


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
                    WinAPI::ComboBox_Add(handle(), label);
                }
            }
            WinAPI::ComboBox_SetSelectedIndex(handle(), 0);
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

        int numItems = WinAPI::ComboBox_Size(handle());
        int dropdownHeight = 0;
        if (numItems > 0)
        {
            dropdownHeight = numItems * WinAPI::ComboBox_ItemHeight(handle(), 0); // use index 0
        }

        // This is usually needed as well, I think :S
        int extraHeight = WinAPI::getSizeDifferenceBetweenWindowRectAndClientRect(handle()).cy;

        NativeControl::move(x, y, w, h + dropdownHeight + extraHeight);
    }


    void MenuList::onContentChanged()
    {
        if (mIsInitialized)
        {
            WinAPI::ComboBox_Clear(handle());
            fillComboBox();
        }
        // else: the init will take care of the initial fill
    }
    
    
    Component * CreateMenuPopup(Component * inComponent, const AttributesMapping & inAttr)
    {
        return new MarginDecorator(new MenuPopup(inComponent, inAttr));
    }


    MenuPopup::MenuPopup(Component * inParent, const AttributesMapping & inAttr) :
        PhonyComponent(inParent, inAttr)
    {
    }


    WinAPI::PopupMenu * MenuPopup::getMenu()
    {
        WinAPI::PopupMenu * popupMenu = new WinAPI::PopupMenu;
        for (size_t idx = 0; idx != getChildCount(); ++idx)
        {
            ElementPtr child = el()->children()[idx];
            if (MenuItem * menuItem = child->component()->downcast<MenuItem>())
            {
                popupMenu->append(new WinAPI::PopupMenuItem(menuItem->componentId(), menuItem->getLabel()));
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
            boost::scoped_ptr<WinAPI::PopupMenu> menu(getMenu());
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


    Component * CreateMenuSeparator(XULWin::Component * inParent, const AttributesMapping & inAttr)
    {
        return new Decorator(new MenuSeparator(inParent, inAttr));
    }


    MenuSeparator::MenuSeparator(Component * inParent, const AttributesMapping & inAttr) :
        Super(inParent, inAttr)
    {
    }

} // namespace XULWin
