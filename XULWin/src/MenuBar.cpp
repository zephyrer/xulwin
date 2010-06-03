#include "XULWin/MenuBar.h"
#include "XULWin/Decorator.h"
#include "XULWin/Defaults.h"
#include "XULWin/Menu.h"
#include "XULWin/MenuItem.h"
#include "XULWin/Window.h"


namespace XULWin
{

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

} // namespace XULWin
