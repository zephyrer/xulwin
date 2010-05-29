#include "XULWin/XMLMenuSeparator.h"
#include "XULWin/MenuSeparator.h"
#include "XULWin/Decorator.h"


namespace XULWin
{

    XMLMenuSeparator::XMLMenuSeparator(Element * inParent, const AttributesMapping & inAttr) :
        Element(XMLMenuSeparator::TagName(),
                inParent,
                new MenuSeparator(inParent->component(), inAttr))
    {
    }

} // namespace XULWin
