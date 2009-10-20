#include "XULWin/ListCell.h"
#include "XULWin/WinUtils.h"
#include "XULWin/Defaults.h"


namespace XULWin
{

    ListCell::ListCell(Component * inParent, const AttributesMapping & inAttributesMapping) :
        Super(inParent, inAttributesMapping)
    {
    }


    bool ListCell::init()
    {
        return Super::init();
    }

    
    bool ListCell::initAttributeControllers()
    {
        setAttributeController("label", static_cast<LabelController*>(this));
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


    std::string ListCell::getLabel() const
    {
        return mLabel;
    }

    
    void ListCell::setLabel(const std::string & inLabel)
    {
        mLabel = inLabel;
    }
    
} // namespace XULWin
