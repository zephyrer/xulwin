#ifndef PHONYCOMPONENT_H_INCLUDED
#define PHONYCOMPONENT_H_INCLUDED


#include "XULWin/VirtualComponent.h"


namespace XULWin
{

    /**
     * PhonyComponent
     *
     * PhonyComponent is a component that is not mapped to a native HWND and that also does not ask
     * for any layout space. It may provide certain information on how to postion other components or
     * how they should behave. 
     * Example: <script>.
     */
    class PhonyComponent : public VirtualComponent
    {
    public:
        typedef VirtualComponent Super;

        PhonyComponent(Component * inParent, const AttributesMapping & inAttributesMapping);

        virtual ~PhonyComponent();

        virtual int calculateWidth(SizeConstraint inSizeConstraint) const
        {
            return 0;
        }

        virtual int calculateHeight(SizeConstraint inSizeConstraint) const
        {
            return 0;
        }
    };

} // namespace XULWin


#endif // PHONYCOMPONENT_H_INCLUDED
