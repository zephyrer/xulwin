#include "XULWin/ListItemElement.h"
#include "XULWin/ListItem.h"
#include "XULWin/AttributeController.h"
#include "XULWin/Decorator.h"


namespace XULWin
{

    ListItemElement::ListItemElement(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(ListItemElement::Type(),
                inParent,
                new ListItemImpl(inParent->component(), inAttributesMapping))
    {
    }


    bool ListItemElement::init()
    {
        return Element::init();
    }

} // namespace XULWin
