#ifndef LABEL_H_INCLUDED
#define LABEL_H_INCLUDED


#include "XULWin/Component.h"


namespace XULWin
{

    class Label : public NativeControl,
                  public virtual StringValueController,
                  public virtual CSSTextAlignController
    {
    public:
        typedef NativeControl Super;

        Label(Component * inParent, const AttributesMapping & inAttributesMapping);

        // StringValueController methods
        virtual std::string getValue() const;

        virtual void setValue(const std::string & inStringValue);

        // CSSTextAlignController methods
        virtual CSSTextAlign getCSSTextAlign() const;

        virtual void setCSSTextAlign(CSSTextAlign inValue);

        virtual bool initAttributeControllers();

        virtual bool initStyleControllers();

        virtual int calculateWidth(SizeConstraint inSizeConstraint) const;

        virtual int calculateHeight(SizeConstraint inSizeConstraint) const;
    };

} // namespace XULWin


#endif // LABEL_H_INCLUDED
