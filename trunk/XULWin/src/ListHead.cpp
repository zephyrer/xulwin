#include "XULWin/ListHead.h"
#include "XULWin/ListHeadImpl.h"
#include "XULWin/AttributeController.h"
#include "XULWin/Decorator.h"


namespace XULWin
{

    ListHead::ListHead(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(ListHead::Type(),
                inParent,
                new ListHeadImpl(inParent->component(), inAttributesMapping))
    {
    }


    bool ListHead::init()
    {
        return Element::init();
    }

} // namespace XULWin
