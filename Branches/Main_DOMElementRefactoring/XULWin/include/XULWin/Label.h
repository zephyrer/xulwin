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

        static const char * TagName() { return "label"; }

        static ComponentPtr Create(Component * inParentComponent, Poco::XML::Element * inDOMElement)
        { return Component::Create<Label>(inParentComponent, inDOMElement); }

        Label(Component * inParent, Poco::XML::Element * inDOMElement);

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
