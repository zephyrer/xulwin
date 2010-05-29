#ifndef CHECKBOX_H_INCLUDED
#define CHECKBOX_H_INCLUDED


#include "XULWin/NativeControl.h"


namespace XULWin
{

    class CheckBox : public NativeControl,
                     public virtual CheckedController
    {
    public:
        typedef NativeControl Super;

        CheckBox(Component * inParent, const AttributesMapping & inAttr);

        // CheckedController methods
        virtual bool isChecked() const;

        virtual void setChecked(bool inChecked);

        virtual bool initAttributeControllers();

        virtual int calculateWidth(SizeConstraint inSizeConstraint) const;

        virtual int calculateHeight(SizeConstraint inSizeConstraint) const;
    };

} // namespace XULWin


#endif // CHECKBOX_H_INCLUDED
