#ifndef COMPONENTUTILITIES_H_INCLUDED
#define COMPONENTUTILITIES_H_INCLUDED


#include "XULWin/Component.h"
#include "XULWin/Decorator.h"


namespace XULWin
{


    class ComponentManager
    {
    public:
        static ComponentManager & Instance();
        
        Component * getComponent(Poco::XML::Node * inElementNode);

    private:
        ComponentManager();
    };


    template<class T>
    T * GetComponent(Poco::XML::Node * inElementNode)
    {
        return ComponentManager::Instance().getComponent(inElementNode)->downcast<T>();
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


#endif // COMPONENTUTILITIES_H_INCLUDED
