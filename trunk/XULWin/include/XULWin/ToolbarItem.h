#ifndef TOOLBARITEM_H_INCLUDED
#define TOOLBARITEM_H_INCLUDED


#include "XULWin/VirtualComponent.h"
#include "XULWin/WindowsToolbarItem.h"


namespace XULWin
{


    // FIXME! Clean this up.
    class MenuPopupContainer
    {
    public:
        virtual void showPopupMenu(RECT inToolbarButtonRect) = 0;
    };


    class ToolbarButton : public VirtualComponent,
                          public Windows::ToolbarDropDown::EventHandler,
                          public MenuPopupContainer,
                          public virtual DisabledController,
                          public virtual LabelController,
                          public virtual CSSListStyleImageController
    {
    public:
        typedef VirtualComponent Super;

        ToolbarButton(Component * inParent, const AttributesMapping & inAttributesMapping);

        virtual bool initAttributeControllers();

        virtual bool initStyleControllers();

        virtual int calculateWidth(SizeConstraint inSizeConstraint) const;

        virtual int calculateHeight(SizeConstraint inSizeConstraint) const;

        // From ToolbarDropDown::EventHandler
        virtual void showToolbarMenu(RECT inToolbarButtonRect);

        // From MenuPopupContainer
        virtual void showPopupMenu(RECT inToolbarButtonRect);

        virtual std::string getLabel() const;

        virtual void setLabel(const std::string & inLabel);

        virtual bool isDisabled() const;

        virtual void setDisabled(bool inDisabled);

        virtual void setCSSListStyleImage(const std::string & inURL);

        virtual const std::string & getCSSListStyleImage() const;

        Windows::ConcreteToolbarItem * nativeItem()
        {
            return mButton;
        }

    private:
        Windows::ConcreteToolbarItem * mButton;
        bool mDisabled;
        std::string mLabel;
        std::string mCSSListStyleImage;
    };

} // namespace XULWin


#endif // TOOLBARITEM_H_INCLUDED
