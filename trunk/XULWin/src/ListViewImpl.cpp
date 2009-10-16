#include "XULWin/ListViewImpl.h"


namespace XULWin
{

    ListViewImpl::ListViewImpl(ElementImpl * inParent,
                               const AttributesMapping & inAttributesMapping) :
        Super(inParent,
              inAttributesMapping,
              WC_LISTVIEW,
              WS_EX_CLIENTEDGE,
              LVS_REPORT)
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
        return 100;
    }

    
    int ListViewImpl::calculateHeight(SizeConstraint inSizeConstraint) const
    {
        return 100;
    }
    
} // namespace XULWin
