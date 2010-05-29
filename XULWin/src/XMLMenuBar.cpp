#include "XULWin/XMLMenuBar.h"
#include "XULWin/MenuBar.h"
#include "XULWin/Decorator.h"


namespace XULWin
{

    XMLMenuBar::XMLMenuBar(Element * inParent, const AttributesMapping & inAttr) :
        Element(XMLMenuBar::TagName(),
                inParent,
                new MenuBar(inParent->component(), inAttr))
    {
    }

} // namespace XULWin
