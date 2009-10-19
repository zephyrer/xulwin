#include "XULWin/MenuBar.h"
#include "XULWin/MenuBarComponent.h"
#include "XULWin/AttributeController.h"
#include "XULWin/Decorator.h"


namespace XULWin
{

    MenuBar::MenuBar(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(MenuBar::Type(),
                inParent,
                new MenuBarComponent(inParent->component(), inAttributesMapping))
    {
    }

} // namespace XULWin
