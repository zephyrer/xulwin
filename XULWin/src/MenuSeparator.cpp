#include "XULWin/MenuSeparator.h"
#include "XULWin/Decorator.h"


namespace XULWin
{

    Component * CreateMenuSeparator(XULWin::Component * inParent, const AttributesMapping & inAttr)
    {
        return new Decorator(new MenuSeparator(inParent, inAttr));
    }


    MenuSeparator::MenuSeparator(Component * inParent, const AttributesMapping & inAttr) :
        Super(inParent, inAttr)
    {
    }

} // namespace XULWin
