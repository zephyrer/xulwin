#include "XULWin/ListCols.h"


namespace XULWin
{

    ListColsImpl::ListColsImpl(Component * inParent, const AttributesMapping & inAttributesMapping) :
        Super(inParent, inAttributesMapping)
    {
    }


    bool ListColsImpl::initComponent()
    {
        return Super::initComponent();
    }

    
    bool ListColsImpl::initAttributeControllers()
    {
        return Super::initAttributeControllers();
    }
        
        
    int ListColsImpl::calculateWidth(SizeConstraint inSizeConstraint) const
    {
        return 1;
    }

    
    int ListColsImpl::calculateHeight(SizeConstraint inSizeConstraint) const
    {
        return 1;
    }
    
} // namespace XULWin
