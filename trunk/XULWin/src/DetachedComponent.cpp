#include "XULWin/DetachedComponent.h"


namespace XULWin
{

    DetachedComponent::DetachedComponent(Component * inParent, const AttributesMapping & inAttributesMapping) :
        ConcreteComponent(inParent)
    {
    }
    
    
    DetachedComponent::~DetachedComponent()
    {
    }


    int DetachedComponent::calculateWidth(SizeConstraint inSizeConstraint) const
    {
        return 0;
    }


    int DetachedComponent::calculateHeight(SizeConstraint inSizeConstraint) const
    {
        return 0;
    }

} // namespace XULWin
