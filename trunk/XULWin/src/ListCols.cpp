#include "XULWin/ListCols.h"
#include "XULWin/ListColsImpl.h"
#include "XULWin/AttributeController.h"
#include "XULWin/Decorator.h"


namespace XULWin
{

    ListCols::ListCols(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(ListCols::Type(),
                inParent,
                new ListColsImpl(inParent->component(), inAttributesMapping))
    {
    }


    bool ListCols::init()
    {
        return Element::init();
    }

} // namespace XULWin
