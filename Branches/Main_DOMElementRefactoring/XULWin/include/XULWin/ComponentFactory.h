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
            const char * tagName = ComponentT::TagName();
            
            FactoryMethods::iterator it = mFactoryMethods.find(tagName);
            if (it != mFactoryMethods.end())
            {
                throw std::logic_error("Component with tagname '" + std::string(tagName) + "' has already been registered.");
            }

            FactoryMethod obj = boost::bind(&ComponentT::Create, _1, _2);
            mFactoryMethods.insert(std::make_pair(tagName, obj));
        }

        ComponentPtr create(Component * inParent, Poco::XML::Element * inElement)
        {
            ComponentPtr result;
            FactoryMethods::iterator it = mFactoryMethods.find(inElement->tagName());
            if (it != mFactoryMethods.end())
            {
                const FactoryMethod & factoryMethod(it->second);
                result = factoryMethod(inParent, inElement);
                if (result)
                {
                    ComponentManager::Instance().addComponent(inElement, result.get());
                }
            }
            return result;
        }

    private:
        typedef boost::function<ComponentPtr(Component * inParent, Poco::XML::Element * inElement)> FactoryMethod;
        typedef std::map<std::string, FactoryMethod> FactoryMethods;
        FactoryMethods mFactoryMethods;
    };
    
    static void GetStyles(Poco::XML::Element * inDOMElement, StylesMapping & styles);

    static CSSOverflow GetOverflow(const StylesMapping & inStyles, const std::string & inOverflow);


    /**
     * CreateDecoratedComponent
     * @DecoratorType: decorator type, for example Decorator or MarginDecorator.
     * @ComponentType: native container type, for example Button.
     * @inParent: the parent element of the to be created component.
     * @inDOMElement: the attributes
     *
     * Factory method for component creation.
     */
    template<class DecoratorType, class ComponentType>
    Component * CreateDecoratedComponent(Component * inParent, Poco::XML::Element * inDOMElement)
    {
        Component * result(0);
        //if (!CreateToolbarChild<DecoratorType, ComponentType>(inParent, inAttributes, result))
        {
            result = new DecoratorType(new ComponentType(inParent, inAttributes));
        }
        return result;
    }


    /**
     * createContainer
     * @VirtualType: virtual container type, for example VirtualGrid.
     * @Type: native container type, for example NativeGrid.
     * @inParent: the parent element of the to be created component.
     * @inDOMElement: the attributes
     *
     * Factory method for container creation.
     * Native container type is created in case scrollbars need to be added.
     * This is because it's very difficult to scroll virtual containers.
     */
    template<class DecoratorType, class VirtualType, class NativeType>
    Component * CreateDecoratedContainerComponent(Component * inParent, Poco::XML::Element * inDOMElement)
    {
        StylesMapping styles;
        GetStyles(inDOMElement, styles);
        CSSOverflow overflowX = GetOverflow(styles, "overflow-x");
        CSSOverflow overflowY = GetOverflow(styles, "overflow-y");
        if (overflowX != CSSOverflow_Hidden || overflowY != CSSOverflow_Hidden)
        {
            return new ScrollDecorator(inParent,
                                       createComponent<DecoratorType, NativeType>(inParent, inDOMElement),
                                       overflowX,
                                       overflowY);
        }
        else
        {
            return CreateDecoratedComponent<DecoratorType, VirtualType>(inParent, inDOMElement);
        }
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
