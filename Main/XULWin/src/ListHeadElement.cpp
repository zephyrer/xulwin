#include "XULWin/ListHeadElement.h"
#include "XULWin/ListHead.h"
#include "XULWin/AttributeController.h"
#include "XULWin/Decorator.h"


namespace XULWin
{

    ListHeadElement::ListHeadElement(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(ListHeadElement::TagName(),
                inParent,
                new ListHead(inParent->component(), inAttributesMapping))
    {
    }


    bool ListHeadElement::init()
    {
        return Element::init();
    }

} // namespace XULWin
