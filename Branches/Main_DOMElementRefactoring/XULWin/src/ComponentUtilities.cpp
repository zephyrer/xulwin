#include "XULWin/ComponentUtilities.h"


namespace XULWin
{


    ComponentManager & ComponentManager::Instance()
    {
        static ComponentManager fInstance;
        return fInstance;
    }
        
    
    Component * ComponentManager::getComponent(Poco::XML::Node * inElementNode)
    {
        return 0; // TODO: implement
    }
    
    
    ComponentManager::ComponentManager()
    {
    }

} // namespace XULWin
