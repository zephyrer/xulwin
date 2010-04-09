#ifndef COMPONENTMANAGER_H_INCLUDED
#define COMPONENTMANAGER_H_INCLUDED


#include "XULWin/Component.h"
#include "XULWin/Decorator.h"
#include <map>


namespace XULWin
{


    class ComponentManager
    {
    public:
        static ComponentManager & Instance();
        
        Component * getComponent(Poco::XML::Element * inElementNode);

        void addComponent(Poco::XML::Element * inElementNode, Component * inComponent);

    private:
        ComponentManager();

        typedef std::map<Poco::XML::Element *, Component*> Components;
        Components mComponents;
    };


    template<class T>
    T * GetComponent(Poco::XML::Node * inElementNode)
    {
        Poco::XML::Element * element = dynamic_cast<Poco::XML::Element*>(inElementNode);
        if (element)
        {
            return ComponentManager::Instance().getComponent(element)->downcast<T>();
        }
        return 0;
    }


    template<class T>
    void GetImmediateChildComponents(Component * inComponent, std::vector<T*> & outComponents)
    {        
        for (size_t idx = 0; idx != inComponent->getChildCount(); ++idx)
        {
            if (T * child = inComponent->getChild(idx)->downcast<T>())
            {
                outComponents.push_back(child);
            }
        }
    }


    template<class T>
    void GetAllChildComponents(Component * inComponent, std::vector<T*> & outComponents)
    {    
        GetImmediateChildComponents(inComponent, outComponents); 
        for (size_t idx = 0; idx != outComponents->getChildCount(); ++idx)
        {
            GetImmediateChildComponents(outComponents[idx], outComponents);
        }
    }      


} // namespace XULWin


#endif // COMPONENTMANAGER_H_INCLUDED
