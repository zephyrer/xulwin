#include "XULWin/Button.h"
#include "XULWin/Defaults.h"
#include "XULWin/Windows.h"
#include "XULWin/WinUtils.h"
#include <algorithm>


namespace XULWin
{

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
        std::string text = Windows::getWindowText(handle());
        int minWidth = Windows::getTextSize(handle(), text).cx;
        minWidth += Defaults::textPadding();
        return std::max<int>(minWidth, Defaults::buttonWidth());
    }

} // namespace XULWin
