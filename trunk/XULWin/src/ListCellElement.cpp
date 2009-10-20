#include "XULWin/ListCellElement.h"
#include "XULWin/ListCell.h"
#include "XULWin/AttributeController.h"
#include "XULWin/Decorator.h"


namespace XULWin
{

    ListCellElement::ListCellElement(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(ListCellElement::Type(),
                inParent,
                new ListCell(inParent->component(), inAttributesMapping))
    {
    }


    bool ListCellElement::init()
    {
        return Element::init();
    }

} // namespace XULWin
