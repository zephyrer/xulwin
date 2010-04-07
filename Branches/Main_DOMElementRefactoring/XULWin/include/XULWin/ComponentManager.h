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
    void GetChildComponents(Poco::XML::Element * inElement, std::vector<T*> & outComponents)
    {
        Poco::XML::NodeList * nodes = inElement->getElementsByTagName(T::TagName());
        if (nodes->length() == 0)
        {
            return;
        }
        
        Poco::XML::Node * node = nodes->item(0);
        while (node)
        {
            outComponents.push_back(GetComponent<T>(node));
            node = node->nextSibling();
        }
    }


} // namespace XULWin


#endif // COMPONENTMANAGER_H_INCLUDED
