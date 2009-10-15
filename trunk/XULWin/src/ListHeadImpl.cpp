#include "XULWin/ListHeadImpl.h"


namespace XULWin
{

    ListHeadImpl::ListHeadImpl(ElementImpl * inParent, const AttributesMapping & inAttributesMapping) :
        Super(inParent, inAttributesMapping)
    {
    }


    bool ListHeadImpl::initImpl()
    {
        return Super::initImpl();
    }

    
    bool ListHeadImpl::initAttributeControllers()
    {
        return Super::initAttributeControllers();
    }
        
        
    int ListHeadImpl::calculateWidth(SizeConstraint inSizeConstraint) const
    {
        return 1;
    }

    
    int ListHeadImpl::calculateHeight(SizeConstraint inSizeConstraint) const
    {
        return 1;
    }
    
} // namespace XULWin
