#include "XULWin/ListViewImpl.h"


namespace XULWin
{

    ListViewImpl::ListViewImpl(ElementImpl * inParent,
                               const AttributesMapping & inAttributesMapping) :
        Super(inParent,
              inAttributesMapping,
              WC_LISTVIEW,
              0,
              0)
    {
    }


    bool ListViewImpl::initImpl()
    {
        return Super::initImpl();
    }

    
    bool ListViewImpl::initAttributeControllers()
    {
        return Super::initAttributeControllers();
    }
        
        
    int ListViewImpl::calculateWidth(SizeConstraint inSizeConstraint) const
    {
        return 1;
    }

    
    int ListViewImpl::calculateHeight(SizeConstraint inSizeConstraint) const
    {
        return 1;
    }
    
} // namespace XULWin
