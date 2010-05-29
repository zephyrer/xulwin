#include "XULWin/XMLListCell.h"
#include "XULWin/ListCell.h"
#include "XULWin/Decorator.h"


namespace XULWin
{

    XMLListCell::XMLListCell(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(XMLListCell::TagName(),
                inParent,
                new ListCell(inParent->component(), inAttributesMapping))
    {
    }


    bool XMLListCell::init()
    {
        return Element::init();
    }

} // namespace XULWin
