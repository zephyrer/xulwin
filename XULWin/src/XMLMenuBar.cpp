#include "XULWin/XMLMenuBar.h"
#include "XULWin/MenuBar.h"
#include "XULWin/Decorator.h"


namespace XULWin
{

    XMLMenuBar::XMLMenuBar(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(XMLMenuBar::TagName(),
                inParent,
                new MenuBar(inParent->component(), inAttributesMapping))
    {
    }

} // namespace XULWin
