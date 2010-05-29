#ifndef IMAGINARYCOMPONENT_H_INCLUDED
#define IMAGINARYCOMPONENT_H_INCLUDED


#include "XULWin/VirtualComponent.h"


namespace XULWin
{

    /**
     * ImaginaryComponent
     *
     * ImaginaryComponent is a VirtualComponent that
     * does not ask for any layout space.
     */
    class ImaginaryComponent : public VirtualComponent
    {
    public:
        typedef VirtualComponent Super;

        ImaginaryComponent(Component * inParent, const AttributesMapping & inAttributesMapping);

        virtual ~ImaginaryComponent();

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


#endif // IMAGINARYCOMPONENT_H_INCLUDED
