#include "XULWin/Button.h"
#include "XULWin/MarginDecorator.h"
#include "XULWin/Defaults.h"
#include "XULWin/Windows.h"
#include "XULWin/WinUtils.h"
#include <algorithm>


namespace XULWin
{

    Component * CreateButton(Component * inComponent, const AttributesMapping & inAttr)
    {
        return new MarginDecorator(new Button(inComponent, inAttr));
    }


    Button::Button(Component * inParent, const AttributesMapping & inAttr) :
        NativeControl(inParent,
                      inAttr,
                      TEXT("BUTTON"),
                      0, // exStyle
                      WS_TABSTOP | BS_PUSHBUTTON)
    {
    }


    int Button::calculateHeight(SizeConstraint inSizeConstraint) const
    {
        return Defaults::buttonHeight();
    }


    int Button::calculateWidth(SizeConstraint inSizeConstraint) const
    {
        std::string text = WinAPI::getWindowText(handle());
        int minWidth = WinAPI::getTextSize(handle(), text).cx;
        minWidth += Defaults::textPadding();
        return std::max<int>(minWidth, Defaults::buttonWidth());
    }

} // namespace XULWin
