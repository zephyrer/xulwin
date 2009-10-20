#include "XULWin/ListHeader.h"


namespace XULWin
{

    ListHeader::ListHeader(Component * inParent, const AttributesMapping & inAttributesMapping) :
        Super(inParent, inAttributesMapping)
    {
    }


    bool ListHeader::initComponent()
    {
        return Super::initComponent();
    }

    
    bool ListHeader::initAttributeControllers()
    {
        setAttributeController("label", static_cast<LabelController*>(this));
        return Super::initAttributeControllers();
    }
        
        
    int ListHeader::calculateWidth(SizeConstraint inSizeConstraint) const
    {
        return 1;
    }

    
    int ListHeader::calculateHeight(SizeConstraint inSizeConstraint) const
    {
        return 1;
    }


    std::string ListHeader::getLabel() const
    {
        return mLabel;
    }

    
    void ListHeader::setLabel(const std::string & inLabel)
    {
        mLabel = inLabel;
    }

    
} // namespace XULWin
