#include "XULWin/Menu.h"
#include "XULWin/MenuComponent.h"
#include "XULWin/AttributeController.h"
#include "XULWin/Decorator.h"


namespace XULWin
{

    Menu::Menu(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(Menu::Type(),
                inParent,
                new MenuComponent(inParent->component(), inAttributesMapping))
    {
    }

} // namespace XULWin
