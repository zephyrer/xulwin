#include "XULWin/PhonyComponent.h"


namespace XULWin
{

    PhonyComponent::PhonyComponent(Component * inParent,
                                           const AttributesMapping & inAttributesMapping) :
        VirtualComponent(inParent, inAttributesMapping)
    {
    }


    PhonyComponent::~PhonyComponent()
    {

    }

} // namespace XULWin
