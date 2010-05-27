#ifndef ELEMENTFACTOR_H_INCLUDED
#define ELEMENTFACTOR_H_INCLUDED


#include "XULWin/Element.h"
#include <boost/bind.hpp>
#include <boost/function.hpp>


namespace XULWin
{

    class ElementFactory
    {
    public:

        static ElementFactory & Instance();

        /**
         * createElement
         *
         * Factory method for the creation of a XUL Element object based on it's XUL type identifier.
         */
        ElementPtr createElement(const std::string & inType, Element * inParent, const AttributesMapping & inAttr);


        /**
         * registerElement
         *
         * This method adds a mapping for the given element's XUL type identifier
         * with the element's factory method (in the form of a function object).
         *
         *
         * This results in a lookup table that looks like this:
         *
         *   "label"     =>   LabelElement::Create(...)
         *   "button"    =>   ButtonElement::Create(...)
         *   "checkbox"  =>   CheckBoxElement::Create(...)
         *   ...
         *
         * Because the XUL type identifier is defined as a class property it suffices
         * to pass the classname in the form of a template parameter. For example:
         *
         * ElementFactory::Instance().registerElement<LabelElement>();
         */
        template<class ElementType>
        void registerElement()
        {
            mFactoryMethods.insert(std::make_pair(ElementType::TagName(),
                                                  boost::bind(ElementType::Create, _1, _2)));
        }

    private:
        ElementFactory();
        typedef boost::function<ElementPtr(Element *, const AttributesMapping &)> FactoryMethod;
        typedef std::map<std::string, FactoryMethod> FactoryMethods;
        FactoryMethods mFactoryMethods;
    };

} // namespace XULWin


#endif // ELEMENTFACTOR_H_INCLUDED
