#include "XULWin/MenuItem.h"
#include "XULWin/WinUtils.h"


namespace XULWin
{

    MenuItem::MenuItemsById MenuItem::sMenuItemsById;


    MenuItem::MenuItem(Component * inParent, const AttributesMapping & inAttr) :
        VirtualComponent(inParent, inAttr)
    {
        assert(sMenuItemsById.find(mComponentId.value()) == sMenuItemsById.end());
        sMenuItemsById.insert(std::make_pair(mComponentId.value(), this));
    }


    MenuItem::~MenuItem()
    {
        MenuItemsById::iterator itById = sMenuItemsById.find(mComponentId.value());
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
        //int result = 0;
        //if (NativeComponent * comp = NativeControl::GetThisOrParent(const_cast<MenuItem *>(this)))
        //{
        //    result = Windows::getTextSize(comp->handle(), getLabel()).cx;
        //}
        //return result;

        // FIXME! 
        return 80;
    }


    int MenuItem::calculateHeight(SizeConstraint inSizeConstraint) const
    {
        //if (NativeComponent * comp = NativeControl::GetThisOrParent(const_cast<MenuItem *>(this)))
        //{
        //    return Windows::getTextSize(comp->handle(), getLabel()).cy;
        //}

        // FIXME!!
        return 80;
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
