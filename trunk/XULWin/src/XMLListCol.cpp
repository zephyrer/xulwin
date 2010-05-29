#include "XULWin/XMLListCol.h"
#include "XULWin/ListCol.h"
#include "XULWin/Decorator.h"


namespace XULWin
{

    XMLListCol::XMLListCol(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(XMLListCol::TagName(),
                inParent,
                new ListCol(inParent->component(), inAttributesMapping))
    {
    }


    bool XMLListCol::init()
    {
        return Element::init();
    }

} // namespace XULWin
