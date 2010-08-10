#ifndef TOOLBAR_H_INCLUDED
#define TOOLBAR_H_INCLUDED


#include "XULWin/Decorator.h"
#include "XULWin/GdiplusLoader.h"
#include "XULWin/NativeControl.h"
#include "XULWin/VirtualComponent.h"
#include "XULWin/WindowsToolbar.h"
#include "XULWin/WindowsToolbarItem.h"


namespace XULWin
{


    class Toolbar : public NativeControl,
                    public WinAPI::WindowsToolbar::EventHandler,
                    public GdiplusLoader
    {
    public:
        typedef NativeControl Super;

        Toolbar(Component * inParent, const AttributesMapping & inAttr);

        virtual ~Toolbar();

        virtual bool init();

        virtual int calculateWidth(SizeConstraint inSizeConstraint) const;

        virtual int calculateHeight(SizeConstraint inSizeConstraint) const;

        virtual void rebuildLayout();

        // XMLToolbar::EventHandler methods
        virtual void onRequestFocus() {}

        boost::shared_ptr<WinAPI::WindowsToolbar> nativeToolbar() const
        {
            return mToolbar;
        }

    private:
        boost::shared_ptr<WinAPI::WindowsToolbar> mToolbar;
    };


    // FIXME! Clean this up.
    class MenuPopupContainer
    {
    public:
        virtual void showPopupMenu(RECT inToolbarButtonRect) = 0;
    };


    class ToolbarButton : public VirtualComponent,
                          public WinAPI::ToolbarDropDown::EventHandler,
                          public MenuPopupContainer,
                          public virtual DisabledController,
                          public virtual LabelController,
                          public virtual CSSListStyleImageController
    {
    public:
        typedef VirtualComponent Super;

        ToolbarButton(Component * inParent, const AttributesMapping & inAttr);

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

        WinAPI::ConcreteToolbarItem * nativeItem()
        {
            return mButton;
        }

    private:
        WinAPI::ConcreteToolbarItem * mButton;
        bool mDisabled;
        std::string mLabel;
        std::string mCSSListStyleImage;
    };


    class ToolbarSpacer : public VirtualComponent
    {
    public:
        typedef VirtualComponent Super;

        ToolbarSpacer(Component * inParent, const AttributesMapping & inAttr);

        virtual int calculateWidth(SizeConstraint inSizeConstraint) const;

        virtual int calculateHeight(SizeConstraint inSizeConstraint) const;

    private:
        WinAPI::ConcreteToolbarItem * mSpacer;
    };


    class ToolbarCustomWindowDecorator : public Decorator,
                                         public WinAPI::IECustomWindow
    {
    public:
        ToolbarCustomWindowDecorator(Component * inDecoratedComponent, boost::weak_ptr<WinAPI::WindowsToolbar> inToolbar);

        virtual UInt32 componentId() const;

        virtual bool hasFocus() const;

        virtual void setFocus() const;

        virtual HWND handle() const;

        virtual bool init();

    private:

        class ConcreteCustomWindow : public WinAPI::IECustomWindow
        {
        public:
            ConcreteCustomWindow(boost::weak_ptr<WinAPI::WindowsToolbar> inToolbar, UInt32 inComponentId, HWND inHandle);

            virtual bool hasFocus() const;

            virtual void setFocus() const;

            virtual HWND handle() const;

            virtual UInt32 componentId() const;

        private:
            boost::weak_ptr<WinAPI::WindowsToolbar> mToolbar;
            HWND mHandle;
            UInt32 mComponentId;
        };
        ConcreteCustomWindow * mCustomWindow;
    };

} // namespace XULWin


#endif // TOOLBAR_H_INCLUDED
