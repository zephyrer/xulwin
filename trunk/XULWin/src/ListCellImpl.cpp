#include "XULWin/ListCellImpl.h"


namespace XULWin
{

    ListCellImpl::ListCellImpl(ElementImpl * inParent, const AttributesMapping & inAttributesMapping) :
        Super(inParent, inAttributesMapping)
    {
    }


    bool ListCellImpl::initImpl()
    {
        return Super::initImpl();
    }

    
    bool ListCellImpl::initAttributeControllers()
    {
        return Super::initAttributeControllers();
    }
        
        
    int ListCellImpl::calculateWidth(SizeConstraint inSizeConstraint) const
    {
        return 1;
    }

    
    int ListCellImpl::calculateHeight(SizeConstraint inSizeConstraint) const
    {
        return 1;
    }
    
} // namespace XULWin
