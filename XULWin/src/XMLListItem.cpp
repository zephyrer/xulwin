#include "XULWin/XMLListItem.h"
#include "XULWin/ListItem.h"
#include "XULWin/Decorator.h"


namespace XULWin
{

    XMLListItem::XMLListItem(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(XMLListItem::TagName(),
                inParent,
                new ListItem(inParent->component(), inAttributesMapping))
    {
    }


    bool XMLListItem::init()
    {
        return Element::init();
    }

} // namespace XULWin
