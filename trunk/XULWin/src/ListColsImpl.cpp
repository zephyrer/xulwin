#include "XULWin/ListColsImpl.h"


namespace XULWin
{

    ListColsImpl::ListColsImpl(Component * inParent, const AttributesMapping & inAttributesMapping) :
        Super(inParent, inAttributesMapping)
    {
    }


    bool ListColsImpl::initImpl()
    {
        return Super::initImpl();
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
