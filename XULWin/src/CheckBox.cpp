#include "XULWin/CheckBox.h"
#include "XULWin/AttributeController.h"
#include "XULWin/MarginDecorator.h"
#include "XULWin/Defaults.h"
#include "XULWin/WinUtils.h"


namespace XULWin
{

    Component * CreateCheckBox(Component * inComponent, const AttributesMapping & inAttr)
    {
        return new MarginDecorator(new CheckBox(inComponent, inAttr));
    }


    CheckBox::CheckBox(Component * inParent, const AttributesMapping & inAttr) :
        NativeControl(inParent,
                      inAttr,
                      TEXT("BUTTON"),
                      0,
                      WS_TABSTOP | BS_AUTOCHECKBOX)
    {
    }


    int CheckBox::calculateHeight(SizeConstraint inSizeConstraint) const
    {
        return Defaults::controlHeight();
    }


    int CheckBox::calculateWidth(SizeConstraint inSizeConstraint) const
    {
        return Defaults::checkBoxMinimumWidth() + WinAPI::getTextSize(handle(), WinAPI::getWindowText(handle())).cx;
    }


    bool CheckBox::isChecked() const
    {
        return WinAPI::isCheckBoxChecked(handle());
    }


    void CheckBox::setChecked(bool inChecked)
    {
        WinAPI::setCheckBoxChecked(handle(), inChecked);
    }


    bool CheckBox::initAttributeControllers()
    {
        setAttributeController<CheckedController>(this);
        return Super::initAttributeControllers();
    }

} // namespace XULWin
