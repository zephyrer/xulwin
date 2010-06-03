#include "XULWin/ListCell.h"
#include "XULWin/Decorator.h"
#include "XULWin/Defaults.h"
#include "XULWin/WinUtils.h"


namespace XULWin
{

    Component * CreateListCell(XULWin::Component * inParent, const AttributesMapping & inAttr)
    {
        return new Decorator(new ListCell(inParent, inAttr));
    }


    ListCell::ListCell(Component * inParent, const AttributesMapping & inAttr) :
        VirtualComponent(inParent, inAttr)
    {
    }


    bool ListCell::initAttributeControllers()
    {
        setAttributeController<LabelController>(this);
        return Super::initAttributeControllers();
    }


    int ListCell::calculateWidth(SizeConstraint inSizeConstraint) const
    {
        //int result = 0;
        //if (NativeComponent * comp = NativeControl::FindNativeParent(parent()))
        //{
        //    int w = WinAPI::getTextSize(comp->handle(), getLabel()).cx + Defaults::listViewColumnTextPadding();
        //    if (w > result)
        //    {
        //        result = w;
        //    }
        //}
        //return result;

        //FIXME! Implement this correctly!
        return 80;
    }


    int ListCell::calculateHeight(SizeConstraint inSizeConstraint) const
    {
        return 1;
    }

} // namespace XULWin
