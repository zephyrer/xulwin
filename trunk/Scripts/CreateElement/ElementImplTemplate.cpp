#include "XULWin/{{ELEMENTIMPL_NAME}}.h"


namespace XULWin
{

    {{ELEMENTIMPL_NAME}}::{{ELEMENTIMPL_NAME}}(ElementImpl * inParent, const AttributesMapping & inAttributesMapping) :
        Super(inParent, inAttributesMapping)
    {
    }


    bool {{ELEMENTIMPL_NAME}}::initComponent()
    {
        return Super::initComponent();
    }

    
    bool {{ELEMENTIMPL_NAME}}::initAttributeControllers()
    {
        return Super::initAttributeControllers();
    }
        
        
    int {{ELEMENTIMPL_NAME}}::calculateWidth(SizeConstraint inSizeConstraint) const
    {
        return 1;
    }

    
    int {{ELEMENTIMPL_NAME}}::calculateHeight(SizeConstraint inSizeConstraint) const
    {
        return 1;
    }
    
} // namespace XULWin
