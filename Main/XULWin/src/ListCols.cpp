#include "XULWin/ListCols.h"


namespace XULWin
{

    ListCols::ListCols(Component * inParent, const AttributesMapping & inAttributesMapping) :
        Super(inParent, inAttributesMapping)
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
