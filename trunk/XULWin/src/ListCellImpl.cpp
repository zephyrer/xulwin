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
        setAttributeController("label", static_cast<LabelController*>(this));
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


    std::string ListCellImpl::getLabel() const
    {
        return mLabel;
    }

    
    void ListCellImpl::setLabel(const std::string & inLabel)
    {
        mLabel = inLabel;
    }
    
} // namespace XULWin
