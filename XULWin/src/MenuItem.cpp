#include "XULWin/MenuItem.h"
#include "XULWin/WinUtils.h"


namespace XULWin
{

    MenuItem::MenuItemsById MenuItem::sMenuItemsById;


    MenuItem::MenuItem(Component * inParent, const AttributesMapping & inAttributesMapping) :
        PassiveComponent(inParent, inAttributesMapping)
    {
        assert(sMenuItemsById.find(mCommandId.intValue()) == sMenuItemsById.end());
        sMenuItemsById.insert(std::make_pair(mCommandId.intValue(), this));
    }


    MenuItem::~MenuItem()
    {
        MenuItemsById::iterator itById = sMenuItemsById.find(mCommandId.intValue());
        assert(itById != sMenuItemsById.end());
        if (itById != sMenuItemsById.end())
        {
            sMenuItemsById.erase(itById);
        }
    }


    MenuItem * MenuItem::FindById(int inId)
    {
        MenuItemsById::iterator itById = sMenuItemsById.find(inId);
        if (itById != sMenuItemsById.end())
        {
            return itById->second;
        }
        return 0;
    }


    bool MenuItem::initAttributeControllers()
    {
        setAttributeController<LabelController>(this);
        return Super::initAttributeControllers();
    }


    int MenuItem::calculateWidth(SizeConstraint inSizeConstraint) const
    {
        int result = 0;
        if (NativeComponent * comp = NativeControl::GetThisOrParent(const_cast<MenuItem *>(this)))
        {
            result = Windows::getTextSize(comp->handle(), getLabel()).cx;
        }
        return result;
    }


    int MenuItem::calculateHeight(SizeConstraint inSizeConstraint) const
    {
        if (NativeComponent * comp = NativeControl::GetThisOrParent(const_cast<MenuItem *>(this)))
        {
            return Windows::getTextSize(comp->handle(), getLabel()).cy;
        }
        return 0;
    }


    std::string MenuItem::getLabel() const
    {
        return mLabel;
    }


    void MenuItem::setLabel(const std::string & inLabel)
    {
        mLabel = inLabel;
    }

} // namespace XULWin
