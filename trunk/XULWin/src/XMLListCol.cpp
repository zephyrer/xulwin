#include "XULWin/XMLListCol.h"
#include "XULWin/ListCol.h"
#include "XULWin/Decorator.h"


namespace XULWin
{

    XMLListCol::XMLListCol(Element * inParent, const AttributesMapping & inAttr) :
        Element(XMLListCol::TagName(),
                inParent,
                new ListCol(inParent->component(), inAttr))
    {
    }


    bool XMLListCol::init()
    {
        return Element::init();
    }

} // namespace XULWin
