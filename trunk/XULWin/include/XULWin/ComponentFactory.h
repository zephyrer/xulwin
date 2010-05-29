#ifndef COMPONENTFACTORY_H_INCLUDED
#define COMPONENTFACTORY_H_INCLUDED


#include "XULWin/Component.h"
#include "XULWin/Element.h"
#include "XULWin/Toolbar.h"
#include "XULWin/ToolbarCustomWindowDecorator.h"
#include "XULWin/WindowsToolbar.h"
#include "Poco/StringTokenizer.h"
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>


namespace XULWin
{

    class ComponentFactory
    {
    public:
        static ComponentFactory & Instance();

        static void GetStyles(const AttributesMapping & inAttr, StylesMapping & styles);

        static CSSOverflow GetOverflow(const StylesMapping & inStyles, const std::string & inOverflow);

        /**
         * createComponent
         * @DecoratorType: decorator type, for example Decorator or MarginDecorator.
         * @ComponentType: native container type, for example Button.
         * @inParent: the parent element of the to be created component.
         * @inAttr: the attributes
         *
         * Factory method for component creation.
         */
        template<class DecoratorType, class ComponentType>
        Component * createComponent(Component * inParent,
                                    const AttributesMapping & inAttributes)
        {
            Component * result(0);
            if (!CreateToolbarChild<DecoratorType, ComponentType>(inParent, inAttributes, result))
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
         * @inAttr: the attributes
         *
         * Factory method for container creation.
         * Native container type is created in case scrollbars need to be added.
         * This is because it's very difficult to scroll virtual containers.
         */
        template<class DecoratorType, class VirtualType, class NativeType>
        Component * createContainer(Component * inParent, const AttributesMapping & inAttr)
        {
            StylesMapping styles;
            GetStyles(inAttr, styles);
            CSSOverflow overflowX = GetOverflow(styles, "overflow-x");
            CSSOverflow overflowY = GetOverflow(styles, "overflow-y");
            if (overflowX != CSSOverflow_Hidden || overflowY != CSSOverflow_Hidden)
            {
                return new ScrollDecorator(inParent,
                                           createComponent<DecoratorType, NativeType>(inParent, inAttr),
                                           overflowX,
                                           overflowY);
            }
            else
            {
                return createComponent<DecoratorType, VirtualType>(inParent, inAttr);
            }
        }

        template<class DecoratorType, class ComponentType>
        static bool CreateToolbarChild(Component * inParent,
                                       const AttributesMapping & inAttr,
                                       Component *& result)
        {
            if (!inParent)
            {
                return false;
            }
            if (XULWin::Toolbar * toolbar = inParent->downcast<XULWin::Toolbar>())
            {
                DecoratorType * decoratedComponent = new DecoratorType(new ComponentType(inParent, inAttr));
                boost::weak_ptr<Windows::Toolbar> theNativeToolbar(toolbar->nativeToolbar());
                result = new ToolbarCustomWindowDecorator(decoratedComponent, theNativeToolbar);
                return true;
            }
            return false;
        }
    };

    template<class T>
    Component * CreateComponent(Component * inParent, const AttributesMapping & inAttributes)
    {
        return ComponentFactory::Instance().createComponent<XULWin::MarginDecorator, T>(inParent, inAttributes);
    }

} // namespace XULWin


#endif // COMPONENTFACTORY_H_INCLUDED
