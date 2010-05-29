#include "XULWin/XMLListCell.h"
#include "XULWin/ListCell.h"
#include "XULWin/Decorator.h"


namespace XULWin
{

    XMLListCell::XMLListCell(Element * inParent, const AttributesMapping & inAttr) :
        Element(XMLListCell::TagName(),
                inParent,
                new ListCell(inParent->component(), inAttr))
    {
    }


    bool XMLListCell::init()
    {
        return Element::init();
    }

} // namespace XULWin
