#include "XULWin/MenuSeparator.h"


namespace XULWin
{

    MenuSeparator::MenuSeparator(Component * inParent, const AttributesMapping & inAttributesMapping) :
        Super(inParent, inAttributesMapping)
    {
    }


    bool MenuSeparator::init()
    {
        return Super::init();
    }

    
    bool MenuSeparator::initAttributeControllers()
    {
        return Super::initAttributeControllers();
    }
        
        
    int MenuSeparator::calculateWidth(SizeConstraint inSizeConstraint) const
    {
        return 1;
    }

    
    int MenuSeparator::calculateHeight(SizeConstraint inSizeConstraint) const
    {
        return 1;
    }
    
} // namespace XULWin
