#include "XULWin/Menu.h"


namespace XULWin
{
    
    Menu::MenusById Menu::sMenusById;

    Menu::Menu(Component * inParent, const AttributesMapping & inAttributesMapping) :
        PassiveComponent(inParent, inAttributesMapping)
    {
        assert(sMenusById.find(mCommandId.intValue()) == sMenusById.end());
        sMenusById.insert(std::make_pair(mCommandId.intValue(), this));  
    }


    Menu::~Menu()
    {            
        MenusById::iterator itById = sMenusById.find(mCommandId.intValue());
        assert(itById != sMenusById.end());
        if (itById != sMenusById.end())
        {
            sMenusById.erase(itById);
        }
    }
    
    
    Menu * Menu::FindById(int inId)
    {          
        MenusById::iterator itById = sMenusById.find(inId);
        if (itById != sMenusById.end())
        {
            return itById->second;
        }
        return 0;
    }

        
    bool Menu::initAttributeControllers()
    {
        setAttributeController("label", static_cast<LabelController*>(this));
        return Super::initAttributeControllers();
    }
    
    
    std::string Menu::getLabel() const
    {
        return mLabel;
    }

     
    void Menu::setLabel(const std::string & inLabel)
    {
        mLabel = inLabel;
    }
    
} // namespace XULWin
