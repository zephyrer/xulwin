#include "XULWin/ComponentManager.h"


namespace XULWin
{


    ComponentManager & ComponentManager::Instance()
    {
        static ComponentManager fInstance;
        return fInstance;
    }


    Component * ComponentManager::getComponent(Poco::XML::Element * inElementNode)
    {
        Components::iterator it = mComponents.find(inElementNode);
        if (it != mComponents.end())
        {
            return it->second;
        }
        return 0;
    }


    void ComponentManager::addComponent(Poco::XML::Element * inElementNode, Component * inComponent)
    {
        Components::iterator it = mComponents.find(inElementNode);
        if (it != mComponents.end())
        {
            throw std::logic_error("Element-Component mapping already exists.");
        }

        mComponents.insert(std::make_pair(inElementNode, inComponent));
    }
    
    
    ComponentManager::ComponentManager()
    {
    }

} // namespace XULWin
