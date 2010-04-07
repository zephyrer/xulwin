#include "XULWin/Label.h"
#include "XULWin/WinUtils.h"


namespace XULWin
{


    Label::Label(Component * inParent, Poco::XML::Element * inDOMElement) :
        NativeControl(inParent,
                      inDOMElement,
                      TEXT("STATIC"),
                      0, // exStyle
                      SS_LEFT)
    {
    }


    std::string Label::getValue() const
    {
        return Windows::getWindowText(handle());
    }


    void Label::setValue(const std::string & inStringValue)
    {
        Windows::setWindowText(handle(), inStringValue);
    }


    CSSTextAlign Label::getCSSTextAlign() const
    {
        LONG styles = Windows::getWindowStyles(handle());
        if (styles & SS_LEFT)
        {
            return CSSTextAlign_Left;
        }
        else if (styles & SS_CENTER)
        {
            return CSSTextAlign_Center;
        }
        else if (styles & SS_RIGHT)
        {
            return CSSTextAlign_Right;
        }
        else
        {
            return CSSTextAlign_Left;
        }
    }


    void Label::setCSSTextAlign(CSSTextAlign inValue)
    {
        LONG styles = Windows::getWindowStyles(handle());
        styles &= ~SS_LEFT;
        styles &= ~SS_CENTER;
        styles &= ~SS_RIGHT;

        switch (inValue)
        {
            case CSSTextAlign_Left:
            {
                styles |= SS_LEFT;
                break;
            }
            case CSSTextAlign_Center:
            {
                styles |= SS_CENTER;
                break;
            }
            case CSSTextAlign_Right:
            {
                styles |= SS_RIGHT;
                break;
            }
            //case CSSTextAlign_Justify:
            //{
            //    styles |= 0;
            //    break;
            //}
        }
        Windows::setWindowStyle(handle(), styles);
    }


    bool Label::initAttributeControllers()
    {
        setAttributeController<StringValueController>(this);
        return Super::initAttributeControllers();
    }


    bool Label::initStyleControllers()
    {
        setStyleController<CSSTextAlignController>(this);
        return Super::initStyleControllers();
    }


    int Label::calculateWidth(SizeConstraint inSizeConstraint) const
    {
        std::string text = Windows::getWindowText(handle());
        int width = Windows::getTextSize(handle(), text).cx;
        return width;
    }


    int Label::calculateHeight(SizeConstraint inSizeConstraint) const
    {
        return Windows::getTextSize(handle(), Windows::getWindowText(handle())).cy;
    }

} // namespace XULWin
