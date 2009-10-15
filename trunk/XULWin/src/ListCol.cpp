#include "XULWin/ListCol.h"
#include "XULWin/ListColImpl.h"
#include "XULWin/AttributeController.h"
#include "XULWin/Decorator.h"


namespace XULWin
{

    ListCol::ListCol(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(ListCol::Type(),
                inParent,
                new ListColImpl(inParent->impl(), inAttributesMapping))
    {
    }


    bool ListCol::init()
    {
        return Element::init();
    }

} // namespace XULWin
