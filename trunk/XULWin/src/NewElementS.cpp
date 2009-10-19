#include "XULWin/NewElementS.h"


namespace XULWin
{

    NewElementSImpl::NewElementSImpl(ElementImpl * inParent, const AttributesMapping & inAttributesMapping) :
        Super(inParent, inAttributesMapping)
    {
    }


    bool NewElementSImpl::initImpl()
    {
        return Super::initImpl();
    }

    
    bool NewElementSImpl::initAttributeControllers()
    {
        return Super::initAttributeControllers();
    }
        
        
    int NewElementSImpl::calculateWidth(SizeConstraint inSizeConstraint) const
    {
        return 1;
    }

    
    int NewElementSImpl::calculateHeight(SizeConstraint inSizeConstraint) const
    {
        return 1;
    }
    
} // namespace XULWin
