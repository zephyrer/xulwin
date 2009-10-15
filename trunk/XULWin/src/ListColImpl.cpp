#include "XULWin/ListColImpl.h"


namespace XULWin
{

    ListColImpl::ListColImpl(ElementImpl * inParent, const AttributesMapping & inAttributesMapping) :
        Super(inParent, inAttributesMapping)
    {
    }
        
        
    int ListColImpl::calculateWidth(SizeConstraint inSizeConstraint) const
    {
        return 1;
    }

    
    int ListColImpl::calculateHeight(SizeConstraint inSizeConstraint) const
    {
        return 1;
    }


    bool ListColImpl::initImpl()
    {
        return Super::initImpl();
    }
    
} // namespace XULWin
