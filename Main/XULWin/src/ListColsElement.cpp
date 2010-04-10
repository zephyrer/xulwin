#include "XULWin/ListColsElement.h"
#include "XULWin/ListCols.h"
#include "XULWin/AttributeController.h"
#include "XULWin/Decorator.h"


namespace XULWin
{

    ListColsElement::ListColsElement(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(ListColsElement::TagName(),
                inParent,
                new ListCols(inParent->component(), inAttributesMapping))
    {
    }


    bool ListColsElement::init()
    {
        return Element::init();
    }

} // namespace XULWin
