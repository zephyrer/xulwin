#include "XULWin/ListItem.h"
#include "XULWin/ListItemImpl.h"
#include "XULWin/AttributeController.h"
#include "XULWin/Decorator.h"


namespace XULWin
{

    ListItem::ListItem(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(ListItem::Type(),
                inParent,
                new ListItemImpl(inParent->component(), inAttributesMapping))
    {
    }


    bool ListItem::init()
    {
        return Element::init();
    }

} // namespace XULWin
