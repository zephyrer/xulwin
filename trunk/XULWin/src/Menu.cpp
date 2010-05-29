#include "XULWin/Menu.h"
#include "XULWin/Decorator.h"
#include "XULWin/MenuItem.h"
#include "XULWin/MenuSeparator.h"
#include "XULWin/MenuPopup.h"


namespace XULWin
{

    Menu::MenusById Menu::sMenusById;

    Menu::Menu(Component * inParent, const AttributesMapping & inAttributesMapping) :
        PassiveComponent(inParent, inAttributesMapping)
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


    Windows::MenuNode * Menu::FromMenu(const Menu * inMenu)
    {
        // XUL hierarchy:
        // menu/menupopup/menuitem
        //               /menu
        Windows::MenuNode * result = new Windows::MenuNode(
            Windows::MenuItemInfo(inMenu->componentId(),
                                  inMenu->getLabel()));
        const MenuPopup * popup = inMenu->findChildOfType<MenuPopup>();
        for (size_t idx = 0; idx != popup->getChildCount(); ++idx)
        {
            const Component * comp = popup->getChild(idx);
            if (const MenuItem * menuItem = comp->downcast<MenuItem>())
            {
                result->addChild(
                    new Windows::MenuNode(
                        Windows::MenuItemInfo(menuItem->componentId(),
                                              menuItem->getLabel())));
            }
            else if (const Menu * menu = comp->downcast<Menu>())
            {
                result->addChild(FromMenu(menu));
            }
            else if (const MenuSeparator * sep = comp->downcast<MenuSeparator>())
            {
                result->addChild(new Windows::MenuNode(Windows::MenuItemInfo(0, "")));
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

} // namespace XULWin
