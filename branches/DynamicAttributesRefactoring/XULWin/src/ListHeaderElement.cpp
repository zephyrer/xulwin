#include "XULWin/ListHeaderElement.h"
#include "XULWin/ListHeader.h"
#include "XULWin/AttributeController.h"
#include "XULWin/Decorator.h"


namespace XULWin
{

    ListHeaderElement::ListHeaderElement(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(ListHeaderElement::Type(),
                inParent,
                new ListHeader(inParent->component(), inAttributesMapping))
    {
    }


    bool ListHeaderElement::init()
    {
        return Element::init();
    }

} // namespace XULWin
