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
        return Defaults::checkBoxMinimumWidth() + Windows::getTextSize(handle(), Windows::getWindowText(handle())).cx;
    }


    bool CheckBox::isChecked() const
    {
        return Windows::isCheckBoxChecked(handle());
    }


    void CheckBox::setChecked(bool inChecked)
    {
        Windows::setCheckBoxChecked(handle(), inChecked);
    }


    bool CheckBox::initAttributeControllers()
    {
        setAttributeController<CheckedController>(this);
        return Super::initAttributeControllers();
    }

} // namespace XULWin
