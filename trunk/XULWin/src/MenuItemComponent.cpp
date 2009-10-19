#include "XULWin/MenuItemComponent.h"
#include "XULWin/WinUtils.h"


namespace XULWin
{
    
    MenuItemComponent::MenuItemsById MenuItemComponent::sMenuItemsById;

    
    MenuItemComponent::MenuItemComponent(Component * inParent, const AttributesMapping & inAttributesMapping) :
        PassiveComponent(inParent, inAttributesMapping)
    {
        assert (sMenuItemsById.find(mCommandId.intValue()) == sMenuItemsById.end());
        sMenuItemsById.insert(std::make_pair(mCommandId.intValue(), this));
    }

    
    MenuItemComponent::~MenuItemComponent()
    {        
        MenuItemsById::iterator itById = sMenuItemsById.find(mCommandId.intValue());
        assert (itById != sMenuItemsById.end());
        if (itById != sMenuItemsById.end())
        {
            sMenuItemsById.erase(itById);
        }
    }

    
    MenuItemComponent * MenuItemComponent::FindById(int inId)
    {
        MenuItemsById::iterator itById = sMenuItemsById.find(inId);
        if (itById != sMenuItemsById.end())
        {
            return itById->second;
        }
        return 0;
    }

        
    bool MenuItemComponent::initAttributeControllers()
    {
        setAttributeController("label", static_cast<LabelController*>(this));
        return Super::initAttributeControllers();
    }
            
    
    int MenuItemComponent::calculateWidth(SizeConstraint inSizeConstraint) const
    {
        if (NativeComponent * comp = NativeControl::GetNativeThisOrParent(const_cast<MenuItemComponent*>(this)))
        {
            return Windows::getTextSize(comp->handle(), getLabel()).cx;
        }
        return 0;
    }


    int MenuItemComponent::calculateHeight(SizeConstraint inSizeConstraint) const
    {
        if (NativeComponent * comp = NativeControl::GetNativeThisOrParent(const_cast<MenuItemComponent*>(this)))
        {
            return Windows::getTextSize(comp->handle(), getLabel()).cy;
        }
        return 0;
    }


    std::string MenuItemComponent::getLabel() const
    {
        return mLabel;
    }

    
    void MenuItemComponent::setLabel(const std::string & inLabel)
    {
        mLabel = inLabel;
    }
    
} // namespace XULWin
