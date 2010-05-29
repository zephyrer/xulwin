#include "XULWin/ImaginaryComponent.h"


namespace XULWin
{

    ImaginaryComponent::ImaginaryComponent(Component * inParent,
                                           const AttributesMapping & inAttributesMapping) :
        VirtualComponent(inParent, inAttributesMapping)
    {
    }


    ImaginaryComponent::~ImaginaryComponent()
    {

    }

} // namespace XULWin
