#include "XULWin/ListCols.h"
#include "XULWin/Decorator.h"


namespace XULWin
{

    Component * CreateListCols(XULWin::Component * inParent, const AttributesMapping & inAttr)
    {
        return new Decorator(new ListCols(inParent, inAttr));
    }


    ListCols::ListCols(Component * inParent, const AttributesMapping & inAttr) :
        Super(inParent, inAttr)
    {
    }


    bool ListCols::init()
    {
        return Super::init();
    }


    bool ListCols::initAttributeControllers()
    {
        return Super::initAttributeControllers();
    }


    int ListCols::calculateWidth(SizeConstraint inSizeConstraint) const
    {
        return 1;
    }


    int ListCols::calculateHeight(SizeConstraint inSizeConstraint) const
    {
        return 1;
    }

} // namespace XULWin
