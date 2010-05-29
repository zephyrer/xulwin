#include "XULWin/XMLListHeader.h"
#include "XULWin/ListHeader.h"
#include "XULWin/Decorator.h"


namespace XULWin
{

    XMLListHeader::XMLListHeader(Element * inParent, const AttributesMapping & inAttr) :
        Element(XMLListHeader::TagName(),
                inParent,
                new ListHeader(inParent->component(), inAttr))
    {
    }


    bool XMLListHeader::init()
    {
        return Element::init();
    }

} // namespace XULWin
