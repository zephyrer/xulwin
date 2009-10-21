#include "XULWin/{{COMPONENT_NAME}}.h"


namespace XULWin
{

    {{COMPONENT_NAME}}::{{COMPONENT_NAME}}(Component * inParent, const AttributesMapping & inAttributesMapping) :
        Super(inParent, inAttributesMapping)
    {
    }


    bool {{COMPONENT_NAME}}::init()
    {
        return Super::init();
    }

    
    bool {{COMPONENT_NAME}}::initAttributeControllers()
    {
        return Super::initAttributeControllers();
    }
        
        
    int {{COMPONENT_NAME}}::calculateWidth(SizeConstraint inSizeConstraint) const
    {
        return 1;
    }

    
    int {{COMPONENT_NAME}}::calculateHeight(SizeConstraint inSizeConstraint) const
    {
        return 1;
    }
    
} // namespace XULWin
