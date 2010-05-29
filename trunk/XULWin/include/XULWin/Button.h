#ifndef BUTTON_H_INCLUDED
#define BUTTON_H_INCLUDED


#include "NativeControl.h"


namespace XULWin
{

    /**
     * Button
     *
     * Native implementation for a XUL button.
     */
    class Button : public NativeControl
    {
    public:
        typedef NativeControl Super;

        Button(Component * inParent, const AttributesMapping & inAttr);

        virtual int calculateWidth(SizeConstraint inSizeConstraint) const;

        virtual int calculateHeight(SizeConstraint inSizeConstraint) const;
    };

} // namespace XULWin


#endif // BUTTON_H_INCLUDED
