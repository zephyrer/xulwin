#include "XULWin/{{ELEMENTIMPL_NAME}}.h"


namespace XULWin
{

    {{ELEMENTIMPL_NAME}}::{{ELEMENTIMPL_NAME}}(ElementImpl * inParent, const AttributesMapping & inAttributesMapping) :
        Super(inParent, inAttributesMapping)
    {
    }
        
        
    int {{ELEMENTIMPL_NAME}}::calculateWidth(SizeConstraint inSizeConstraint) const
    {
        return 1;
    }

    
    int {{ELEMENTIMPL_NAME}}::calculateHeight(SizeConstraint inSizeConstraint) const
    {
        return 1;
    }


    bool {{ELEMENTIMPL_NAME}}::initImpl()
    {
        return Super::initImpl();
    }
    
} // namespace XULWin
