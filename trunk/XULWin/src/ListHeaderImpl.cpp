#include "XULWin/ListHeaderImpl.h"


namespace XULWin
{

    ListHeaderImpl::ListHeaderImpl(Component * inParent, const AttributesMapping & inAttributesMapping) :
        Super(inParent, inAttributesMapping)
    {
    }


    bool ListHeaderImpl::initComponent()
    {
        return Super::initComponent();
    }

    
    bool ListHeaderImpl::initAttributeControllers()
    {
        setAttributeController("label", static_cast<LabelController*>(this));
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


    std::string ListHeaderImpl::getLabel() const
    {
        return mLabel;
    }

    
    void ListHeaderImpl::setLabel(const std::string & inLabel)
    {
        mLabel = inLabel;
    }

    
} // namespace XULWin
