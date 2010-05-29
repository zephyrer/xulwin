#include "XULWin/XMLListHead.h"
#include "XULWin/ListHead.h"
#include "XULWin/Decorator.h"


namespace XULWin
{

    XMLListHead::XMLListHead(Element * inParent, const AttributesMapping & inAttr) :
        Element(XMLListHead::TagName(),
                inParent,
                new ListHead(inParent->component(), inAttr))
    {
    }


    bool XMLListHead::init()
    {
        return Element::init();
    }

} // namespace XULWin
