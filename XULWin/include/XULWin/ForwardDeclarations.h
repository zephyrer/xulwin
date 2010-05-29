#ifndef FORWARDDECLARATIONS_H_INCLUDED
#define FORWARDDECLARATIONS_H_INCLUDED


#include <boost/shared_ptr.hpp>
#include <vector>


namespace XULWin
{

    class BoxLayouter;    
    class Component;
    class ComponentFactory;
    class Decorator;
    class Element;
    class ElementFactory;
    class EventListener;
    class NativeComponent;
    typedef boost::shared_ptr<Component> ComponentPtr;
    typedef boost::shared_ptr<Element> ElementPtr;
    typedef std::vector<ElementPtr> Children;

} // namespace XULWin


#endif // FORWARDDECLARATIONS_H_INCLUDED
