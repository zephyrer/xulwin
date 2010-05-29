#include "XULWin/ToolbarItem.h"
#include "XULWin/ChromeURL.h"
#include "XULWin/Decorator.h"
#include "XULWin/Defaults.h"
#include "XULWin/Element.h"
#include "XULWin/MenuPopup.h"
#include "XULWin/Toolbar.h"
#include "XULWin/Unicode.h"
#include "XULWin/WinUtils.h"
#include "XULWin/Gdiplus.h"


namespace XULWin
{

    ToolbarButton::ToolbarButton(Component * inParent, const AttributesMapping & inAttr) :
        VirtualComponent(inParent, inAttr),
        mButton(0),
        mDisabled(false)
    {
        if (XULWin::Toolbar * toolbar = parent()->downcast<XULWin::Toolbar>())
        {
            boost::shared_ptr<Gdiplus::Bitmap> nullImage;
            std::string label = getLabel();

            std::string buttonType;
            AttributesMapping::const_iterator it = inAttr.find("type");
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
                mButton = new Windows::ToolbarDropDown(toolbar->nativeToolbar(),
                                                       this,
                                                       mComponentId.value(),
                                                       label,
                                                       label,
                                                       nullImage,
                                                       false);
            }
            else if (buttonType == "menu-button")
            {
                mButton = new Windows::ToolbarDropDown(toolbar->nativeToolbar(),
                                                       this,
                                                       mComponentId.value(),
                                                       label,
                                                       label,
                                                       nullImage,
                                                       true);
            }
            else // buttonType.empty() or buttonType == "button"
            {
                mButton = new Windows::XMLToolbarButton(toolbar->nativeToolbar(),
                                                            mComponentId.value(),
                                                            boost::function<void()>(),
                                                            label,
                                                            label,
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
        if (Toolbar * toolbar = parent()->downcast<Toolbar>())
        {
            int textWidth = Windows::getTextSize(toolbar->handle(), getLabel()).cx;
            int imageWidth = 0;
            if (mButton && mButton->image())
            {
                imageWidth = mButton->image()->GetWidth();
            }
            return std::max<int>(textWidth, imageWidth);

        }
        return 0;
    }


    int ToolbarButton::calculateHeight(SizeConstraint inSizeConstraint) const
    {
        int result = Defaults::toolbarHeight();
        if (Toolbar * toolbar = parent()->downcast<Toolbar>())
        {
            int textHeight = Windows::getTextSize(toolbar->handle(), getLabel()).cy;
            if (textHeight > result)
            {
                result = textHeight;
            }
            if (mButton && mButton->image())
            {
                int imageHeight = mButton->image()->GetHeight();
                if (imageHeight > result)
                {
                    result = imageHeight;
                }
            }
        }
        return result;
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

} // namespace XULWin
