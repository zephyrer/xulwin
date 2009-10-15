#include "XULWin/ListHeaderImpl.h"


namespace XULWin
{

    ListHeaderImpl::ListHeaderImpl(ElementImpl * inParent, const AttributesMapping & inAttributesMapping) :
        Super(inParent, inAttributesMapping)
    {
    }


    bool ListHeaderImpl::initImpl()
    {
        return Super::initImpl();
    }

    
    bool ListHeaderImpl::initAttributeControllers()
    {
        return Super::initAttributeControllers();
    }
        
        
    int ListHeaderImpl::calculateWidth(SizeConstraint inSizeConstraint) const
    {
        return 1;
    }

    
    int ListHeaderImpl::calculateHeight(SizeConstraint inSizeConstraint) const
    {
        return 1;
    }
    
} // namespace XULWin
