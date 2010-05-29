#include "XULWin/XMLListHead.h"
#include "XULWin/ListHead.h"
#include "XULWin/Decorator.h"


namespace XULWin
{

    XMLListHead::XMLListHead(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(XMLListHead::TagName(),
                inParent,
                new ListHead(inParent->component(), inAttributesMapping))
    {
    }


    bool XMLListHead::init()
    {
        return Element::init();
    }

} // namespace XULWin
