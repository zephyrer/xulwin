#include "XULWin/TextBox.h"
#include "XULWin/Decorator.h"
#include "XULWin/Defaults.h"
#include "XULWin/Element.h"
#include "XULWin/WinUtils.h"


namespace XULWin
{

    TextBox::TextBox(Component * inParent, const AttributesMapping & inAttr) :
        NativeControl(inParent,
                      inAttr,
                      TEXT("EDIT"),
                      WS_EX_CLIENTEDGE, // exStyle
                      WS_TABSTOP | GetFlags(inAttr)),
        mRows(1)
    {
    }


    DWORD TextBox::GetFlags(const AttributesMapping & inAttr)
    {
        DWORD flags = 0;
        AttributesMapping::const_iterator it = inAttr.find("type");
        if (it != inAttr.end() && it->second == "password")
        {
            flags |= ES_PASSWORD;
        }
        it = inAttr.find("multiline");
        if (it != inAttr.end() && it->second == "true")
        {
            flags |= WS_VSCROLL | ES_MULTILINE | ES_AUTOVSCROLL | ES_WANTRETURN;
        }
        else
        {
            flags |= ES_AUTOHSCROLL;
        }
        return flags;
    }


    std::string TextBox::getValue() const
    {
        return Windows::getWindowText(handle());
    }


    void TextBox::setValue(const std::string & inStringValue)
    {
        Windows::setWindowText(handle(), inStringValue);
    }


    bool TextBox::isReadOnly() const
    {
        return Windows::isTextBoxReadOnly(handle());
    }


    void TextBox::setReadOnly(bool inReadOnly)
    {
        Windows::setTextBoxReadOnly(handle(), inReadOnly);
    }


    int TextBox::getRows() const
    {
        return mRows;
    }


    void TextBox::setRows(int inRows)
    {
        mRows = inRows;
    }


    bool TextBox::initAttributeControllers()
    {
        setAttributeController<StringValueController>(this);
        setAttributeController<ReadOnlyController>(this);
        setAttributeController<RowsController>(this);
        return Super::initAttributeControllers();
    }


    int TextBox::calculateWidth(SizeConstraint inSizeConstraint) const
    {
        return Defaults::textBoxWidth();;
    }


    int TextBox::calculateHeight(SizeConstraint inSizeConstraint) const
    {
        return Defaults::controlHeight() * getRows();
    }


    LRESULT TextBox::handleMessage(UINT inMessage, WPARAM wParam, LPARAM lParam)
    {
        if (inMessage == WM_KEYDOWN)
        {
            if (wParam == VK_TAB)
            {
                //
                // Tabbing doesn't work on multiline textboxes.
                // This code fixes that.
                //
                long style = ::GetWindowLong(handle(), GWL_STYLE);
                if ((style & ES_MULTILINE) && (style & WS_TABSTOP))
                {
                    BOOL shift = 0x8000 & GetKeyState(VK_SHIFT);
                    ::SetFocus(::GetNextDlgTabItem(GetParent(handle()), handle(), shift));
                    return 0;
                }
            }
            else
            {
                //
                // CTRL-a select all
                //
                const int cKeyboard_a = 65;
                if (HIWORD(::GetKeyState(VK_CONTROL)) && wParam == cKeyboard_a)
                {
                    ::SendMessage(handle(), EM_SETSEL, 0, -1);
                }
            }
        }
        return Super::handleMessage(inMessage, wParam, lParam);
    }

} // namespace XULWin
