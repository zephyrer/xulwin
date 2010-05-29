#include "XULWin/ListCell.h"
#include "XULWin/WinUtils.h"
#include "XULWin/Defaults.h"


namespace XULWin
{

    ListCell::ListCell(Component * inParent, const AttributesMapping & inAttributesMapping) :
        VirtualComponent(inParent, inAttributesMapping)
    {
    }


    bool ListCell::initAttributeControllers()
    {
        setAttributeController<LabelController>(this);
        return Super::initAttributeControllers();
    }


    int ListCell::calculateWidth(SizeConstraint inSizeConstraint) const
    {
        int result = 0;
        if (NativeComponent * comp = NativeControl::GetThisOrParent(parent()))
        {
            int w = Windows::getTextSize(comp->handle(), getLabel()).cx + Defaults::listViewColumnTextPadding();
            if (w > result)
            {
                result = w;
            }
        }
        return result;
    }


    int ListCell::calculateHeight(SizeConstraint inSizeConstraint) const
    {
        return 1;
    }

} // namespace XULWin
