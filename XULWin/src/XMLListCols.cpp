#include "XULWin/XMLListCols.h"
#include "XULWin/ListCols.h"
#include "XULWin/Decorator.h"


namespace XULWin
{

    XMLListCols::XMLListCols(Element * inParent, const AttributesMapping & inAttr) :
        Element(XMLListCols::TagName(),
                inParent,
                new ListCols(inParent->component(), inAttr))
    {
    }


    bool XMLListCols::init()
    {
        return Element::init();
    }

} // namespace XULWin
