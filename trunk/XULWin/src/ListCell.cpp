#include "XULWin/ListCell.h"
#include "XULWin/ListCellImpl.h"
#include "XULWin/AttributeController.h"
#include "XULWin/Decorator.h"


namespace XULWin
{

    ListCell::ListCell(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(ListCell::Type(),
                inParent,
                new ListCellImpl(inParent->component(), inAttributesMapping))
    {
    }


    bool ListCell::init()
    {
        return Element::init();
    }

} // namespace XULWin
