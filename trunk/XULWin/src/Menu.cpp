#include "XULWin/Menu.h"


namespace XULWin
{
    
    MenuComponent::MenusById MenuComponent::sMenusById;

    MenuComponent::MenuComponent(Component * inParent, const AttributesMapping & inAttributesMapping) :
        PassiveComponent(inParent, inAttributesMapping)
    {
        assert(sMenusById.find(mCommandId.intValue()) == sMenusById.end());
        sMenusById.insert(std::make_pair(mCommandId.intValue(), this));  
    }


    MenuComponent::~MenuComponent()
    {            
        MenusById::iterator itById = sMenusById.find(mCommandId.intValue());
        assert(itById != sMenusById.end());
        if (itById != sMenusById.end())
        {
            sMenusById.erase(itById);
        }
    }
    
    
    MenuComponent * MenuComponent::FindById(int inId)
    {          
        MenusById::iterator itById = sMenusById.find(inId);
        if (itById != sMenusById.end())
        {
            return itById->second;
        }
        return 0;
    }

        
    bool MenuComponent::initAttributeControllers()
    {
        setAttributeController("label", static_cast<LabelController*>(this));
        return Super::initAttributeControllers();
    }
    
    
    std::string MenuComponent::getLabel() const
    {
        return mLabel;
    }

     
    void MenuComponent::setLabel(const std::string & inLabel)
    {
        mLabel = inLabel;
    }
    
} // namespace XULWin
