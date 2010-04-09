#ifndef COMPONENTFACTORY_H_INCLUDED
#define COMPONENTFACTORY_H_INCLUDED


#include "XULWin/Component.h"
#include "XULWin/ComponentManager.h"
#include "XULWin/ToolbarCustomWindowDecorator.h"
#include "Poco/DOM/Element.h"
#include "Poco/StringTokenizer.h"
#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/bind.hpp>


namespace XULWin
{

    class ComponentFactory
    {
    public:
        static ComponentFactory & Instance();

        template<class ComponentT>
        void registerComponent()
        {
            registerComp(ComponentT::TagName(), boost::bind(&ComponentT::Create, _1, _2));
        }

        ComponentPtr create(Component * inParent, Poco::XML::Element * inElement);

    private:
        typedef boost::function<ComponentPtr(Component * inParent, Poco::XML::Element * inElement)> FactoryMethod;
        void registerComp(const char * inTagName, const FactoryMethod & inFactoryMethod);
        typedef std::map<std::string, FactoryMethod> FactoryMethods;
        FactoryMethods mFactoryMethods;
    };

    
    void GetStyles(Poco::XML::Element * inDOMElement, StylesMapping & styles);

    CSSOverflow GetOverflow(const StylesMapping & inStyles, const std::string & inOverflow);


    /**
     * CreateComponent
     *
     * Factory method for creating a component.
     */
    template<class ComponentType>
    ComponentPtr CreateComponent(Component * inParent, Poco::XML::Element * inDOMElement)
    {
        ComponentPtr result(new ComponentType(inParent, inAttributes));
        return result;
    }


    /**
     * CreateDecoratedComponent
     *
     * Factory method creating a decorated component.
     */
    template<class DecoratorType, class ComponentType>
    ComponentPtr CreateDecoratedComponent(Component * inParent, Poco::XML::Element * inDOMElement)
    {
        ComponentPtr result;
        //if (!CreateToolbarChild<DecoratorType, ComponentType>(inParent, inAttributes, result))
        //{
            result.reset(new DecoratorType(new ComponentType(inParent, inAttributes)));
        //}
        return result;
    }


    bool IsScrollable(Poco::XML::Element * inDOMElement, CSSOverflow & outOverflowX, CSSOverflow & outOverflowY);


    /**
     * createContainer
     *
     * @VirtualType     virtual container type, for example VirtualGrid.
     * @NativeType      native container type, for example NativeGrid.
     * @inParent        the parent element of the to be created component.
     * @inDOMElement    the attributes
     *
     * Factory method for container creation.
     * Native container type is created in case scrollbars need to be added.
     * This is because it's very difficult to scroll virtual containers.
     */
    template<class VirtualType, class NativeType>
    ComponentPtr CreateContainer(Component * inParent, Poco::XML::Element * inDOMElement)
    {
        ComponentPtr result;
        CSSOverflow overflowX(CSSOverflow_Hidden), overflowY(CSSOverflow_Hidden);
        if (IsScrollable(inDOMElement, overflowX, overflowY))
        {
            result.reset(new ScrollDecorator(new NativeType(inParent, inDOMElement),
                                             overflowX,
                                             overflowY));
        }
        else
        {
            result.reset(new VirtualType(inParent, inDOMElement));
        }
        return result;
    }

    //template<class DecoratorType, class ComponentType>
    //static bool CreateToolbarChild(Component * inParent,
    //                               Poco::XML::Element * inDOMElement,
    //                               Component *& result)
    //{
    //    if (!inParent)
    //    {
    //        return false;
    //    }
    //    if (XULWin::Toolbar * toolbar = inParent->downcast<XULWin::Toolbar>())
    //    {
    //        DecoratorType * decoratedComponent = new DecoratorType(new ComponentType(inParent, inDOMElement));
    //        boost::weak_ptr<Windows::Toolbar> theNativeToolbar(toolbar->nativeToolbar());
    //        result = new ToolbarCustomWindowDecorator(decoratedComponent, theNativeToolbar);
    //        return true;
    //    }
    //    return false;
    //}

} // namespace XULWin


#endif // COMPONENTFACTORY_H_INCLUDED
