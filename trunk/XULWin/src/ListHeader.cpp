#include "XULWin/ListHeader.h"
#include "XULWin/ListHeaderImpl.h"
#include "XULWin/AttributeController.h"
#include "XULWin/Decorator.h"


namespace XULWin
{

    ListHeader::ListHeader(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(ListHeader::Type(),
                inParent,
                new ListHeaderImpl(inParent->impl(), inAttributesMapping))
    {
    }


    bool ListHeader::init()
    {
        return Element::init();
    }

} // namespace XULWin
