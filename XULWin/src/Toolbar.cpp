#include "XULWin/Toolbar.h"
#include "XULWin/ChromeURL.h"
#include "XULWin/Decorator.h"
#include "XULWin/Defaults.h"
#include "XULWin/Element.h"
#include "XULWin/Gdiplus.h"
#include "XULWin/Menu.h"
#include "XULWin/Unicode.h"


namespace XULWin
{


    Component * CreateToolbar(Component * inParent, const AttributesMapping & inAttr)
    {
        return new Decorator(new Toolbar(inParent, inAttr));
    }


    Toolbar::Toolbar(Component * inParent, const AttributesMapping & inAttr) :
        NativeControl(inParent, inAttr)
    {
        if (NativeComponent * native = NativeControl::FindNativeParent(inParent))
        {
            mToolbar.reset(new WinAPI::WindowsToolbar(this, NativeComponent::GetModuleHandle(), native->handle(), mComponentId.value()));
            setHandle(mToolbar->handle(), false);
            registerHandle();
            subclass();
        }
    }


    Toolbar::~Toolbar()
    {
        unsubclass();
        unregisterHandle();
        mToolbar.reset();
    }


    bool Toolbar::init()
    {
        mToolbar->buildToolbar();
        mToolbar->rebuildLayout();
        ShowWindow(mToolbar->handle(), SW_SHOW);
        return Super::init();
    }


    int Toolbar::calculateWidth(SizeConstraint inSizeConstraint) const
    {
        return calculateSumChildWidths(inSizeConstraint);
    }


    int Toolbar::calculateHeight(SizeConstraint inSizeConstraint) const
    {
        int result = calculateMaxChildHeight(inSizeConstraint);
        return result;
    }


    void Toolbar::rebuildLayout()
    {
        mToolbar->rebuildLayout();
    }


    Component * CreateToolbarButton(Component * inParent, const AttributesMapping & inAttr)
    {
        return new Decorator(new ToolbarButton(inParent, inAttr));
    }


    ToolbarButton::ToolbarButton(Component * inParent, const AttributesMapping & inAttr) :
        VirtualComponent(inParent, inAttr),
        mButton(0),
        mDisabled(false)
    {
        if (XULWin::Toolbar * toolbar = parent()->downcast<XULWin::Toolbar>())
        {
            boost::shared_ptr<Gdiplus::Bitmap> nullImage;
            
            std::string label, tooltiptext;            
            AttributesMapping::const_iterator it;
            
            it = inAttr.find("label");
            if (it != inAttr.end())
            {
                label = it->second;
            }

            it = inAttr.find("tooltiptext");
            if (it != inAttr.end())
            {
                tooltiptext = it->second;
            }

            std::string buttonType;
            it = inAttr.find("type");
            if (it != inAttr.end())
            {
                buttonType = it->second;
            }
            //menu: Set the type attribute to the value menu to create a button with a menu popup. Place a menupopup element inside the button in this case. The user may click anywhere on the button to open and close the menu.
            //menu-button: You can also use the value menu-button to create a button with a menu. Unlike the menu type, this type requires the user to press the arrow to open the menu, but a different command may be invoked when the main part of the button is pressed. This type of button would be used for browser's back and forward buttons.
            //checkbox: Use this type to create a toggle button which will switch the checked state each time the button is pressed.
            //radio: Use this type to create a radio button. You can also create a group of toolbarbutton using this type and the attribute group.
            if (buttonType == "menu")
            {
                mButton = new WinAPI::ToolbarDropDown(toolbar->nativeToolbar(),
                                                       this,
                                                       mComponentId.value(),
                                                       label,
                                                       tooltiptext,
                                                       nullImage,
                                                       false);
            }
            else if (buttonType == "menu-button")
            {
                mButton = new WinAPI::ToolbarDropDown(toolbar->nativeToolbar(),
                                                       this,
                                                       mComponentId.value(),
                                                       label,
                                                       tooltiptext,
                                                       nullImage,
                                                       true);
            }
            else // buttonType.empty() or buttonType == "button"
            {
                mButton = new WinAPI::ToolbarButton(toolbar->nativeToolbar(),
                                                            mComponentId.value(),
                                                            boost::function<void()>(),
                                                            label,
                                                            tooltiptext,
                                                            nullImage);
            }
            assert(mButton);
            toolbar->nativeToolbar()->add(mButton);
            // Now that mButton is constructed we can apply any previously set
            // attributes.
            setLabel(mLabel);
            setDisabled(mDisabled);
            setCSSListStyleImage(mCSSListStyleImage);
        }
    }


    void ToolbarButton::showToolbarMenu(RECT inToolbarButtonRect)
    {
        showPopupMenu(inToolbarButtonRect);
    }


    void ToolbarButton::showPopupMenu(RECT inToolbarButtonRect)
    {
        for (size_t idx = 0; idx != getChildCount(); ++idx)
        {
            ElementPtr child = el()->children()[idx];
            if (MenuPopup * popupMenu = child->component()->downcast<MenuPopup>())
            {
                popupMenu->show(inToolbarButtonRect);
            }
        }
    }


    bool ToolbarButton::initAttributeControllers()
    {
        setAttributeController<LabelController>(this);
        setAttributeController<DisabledController>(this);
        return Super::initAttributeControllers();
    }


    bool ToolbarButton::initStyleControllers()
    {
        setStyleController<CSSListStyleImageController>(this);
        return Super::initStyleControllers();
    }


    int ToolbarButton::calculateWidth(SizeConstraint inSizeConstraint) const
    {        
        RECT rc = mButton->getRect();
        return rc.right - rc.left;
    }


    int ToolbarButton::calculateHeight(SizeConstraint inSizeConstraint) const
    {
        RECT rc = mButton->getRect();
        return rc.bottom - rc.top;
    }


    std::string ToolbarButton::getLabel() const
    {
        if (mButton)
        {
            return mButton->text();
        }
        return mLabel;
    }


    void ToolbarButton::setLabel(const std::string & inLabel)
    {
        if (mButton)
        {
            mButton->setText(inLabel);
        }
        mLabel = inLabel;
    }


    bool ToolbarButton::isDisabled() const
    {
        return mDisabled;
    }


    void ToolbarButton::setDisabled(bool inDisabled)
    {
        if (mButton)
        {
            if (Toolbar * toolbar = parent()->downcast<Toolbar>())
            {
                SendMessage(toolbar->handle(), TB_ENABLEBUTTON, (WPARAM)mComponentId.value(), (LPARAM)MAKELONG(inDisabled ? FALSE : TRUE, 0));
            }
        }
        else
        {
            mDisabled = inDisabled;
        }
    }


    void ToolbarButton::setCSSListStyleImage(const std::string & inURL)
    {
        if (mButton)
        {
            ChromeURL chromeURL(inURL);
            std::wstring utf16URL = ToUTF16(chromeURL.convertToLocalPath());
            boost::shared_ptr<Gdiplus::Bitmap> img(new Gdiplus::Bitmap(utf16URL.c_str()));
            mButton->setImage(img);
        }
        mCSSListStyleImage = inURL;
    }


    const std::string & ToolbarButton::getCSSListStyleImage() const
    {
        return mCSSListStyleImage;
    }


    Component * CreateToolbarSpacer(Component * inParent, const AttributesMapping & inAttr)
    {
        return new Decorator(new ToolbarSpacer(inParent, inAttr));
    }
    
    
    ToolbarSpacer::ToolbarSpacer(Component * inParent, const AttributesMapping & inAttr) :
        VirtualComponent(inParent, inAttr),
        mSpacer(0)
    {
        Toolbar * toolbar = inParent->downcast<Toolbar>();
        if (!toolbar)
        {
            return;
        }

        mSpacer = new WinAPI::ToolbarSpring(toolbar->nativeToolbar(), mComponentId.value());
        toolbar->nativeToolbar()->add(mSpacer);
    }

    
    int ToolbarSpacer::calculateWidth(SizeConstraint inSizeConstraint) const
    {
        RECT rc = mSpacer->getRect();
        return rc.right - rc.left;
    }


    int ToolbarSpacer::calculateHeight(SizeConstraint inSizeConstraint) const
    {
        RECT rc = mSpacer->getRect();
        return rc.bottom - rc.top;
    }


    ToolbarCustomWindowDecorator::ToolbarCustomWindowDecorator(Component * inDecoratedComponent, boost::weak_ptr<WinAPI::WindowsToolbar> inToolbar) :
        Decorator(inDecoratedComponent),
        IECustomWindow(),
        mCustomWindow(0) 
    {
        if (NativeComponent * native = mDecoratedComponent->downcast<NativeComponent>())
        {
            mCustomWindow = new ConcreteCustomWindow(inToolbar, mDecoratedComponent->componentId(), native->handle());
        }
    }


    UInt32 ToolbarCustomWindowDecorator::componentId() const
    {
        if (mCustomWindow)
        {
            mCustomWindow->componentId();
        }
        return 0;
    }


    bool ToolbarCustomWindowDecorator::hasFocus() const
    {
        if (mCustomWindow)
        {
            mCustomWindow->hasFocus();
        }
        return false;
    }


    void ToolbarCustomWindowDecorator::setFocus() const
    {
        if (mCustomWindow)
        {
            mCustomWindow->setFocus();
        }
    }


    HWND ToolbarCustomWindowDecorator::handle() const
    {
        if (mCustomWindow)
        {
            return mCustomWindow->handle();
        }
        return 0;
    }


    ToolbarCustomWindowDecorator::ConcreteCustomWindow::ConcreteCustomWindow(boost::weak_ptr<WinAPI::WindowsToolbar> inToolbar, UInt32 inComponentId, HWND inHandle) :
        mToolbar(inToolbar),
        mHandle(inHandle),
        mComponentId(inComponentId)
    {
    }


    bool ToolbarCustomWindowDecorator::ConcreteCustomWindow::hasFocus() const
    {
        return ::GetFocus() == mHandle;
    }


    void ToolbarCustomWindowDecorator::ConcreteCustomWindow::setFocus() const
    {
        ::SetFocus(mHandle);
    }


    HWND ToolbarCustomWindowDecorator::ConcreteCustomWindow::handle() const
    {
        return mHandle;
    }


    UInt32 ToolbarCustomWindowDecorator::ConcreteCustomWindow::componentId() const
    {
        return mComponentId;
    }


    bool ToolbarCustomWindowDecorator::init()
    {
        if (mCustomWindow)
        {
            if (XULWin::Toolbar * toolbar = parent()->downcast<XULWin::Toolbar>())
            {
                mDecoratedComponent->move(0, 0, mDecoratedComponent->getWidth(), mDecoratedComponent->getHeight());
                mDecoratedComponent->rebuildLayout();
                toolbar->nativeToolbar()->add(mCustomWindow);
            }
        }
        return mDecoratedComponent->init();
    }


} // namespace XULWin
