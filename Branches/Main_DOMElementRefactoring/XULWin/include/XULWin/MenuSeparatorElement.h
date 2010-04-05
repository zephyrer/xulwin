#ifndef MENUSEPARATORELEMENT_H_INCLUDED
#define MENUSEPARATORELEMENT_H_INCLUDED


#include "XULWin/Element.h"


namespace XULWin
{

    class MenuSeparatorElement : public Element
    {
    public:
        static ElementPtr Create(Element * inParent, const AttributesMapping & inAttr)
        {
            return Element::Create<MenuSeparatorElement>(inParent, inAttr);
        }

        static const char * Type()
        {
            return "menuseparator";
        }

    private:
        friend class Element;
        MenuSeparatorElement(Element * inParent, const AttributesMapping & inAttributesMapping);
    };

} // namespace XULWin


#endif // MENUSEPARATORELEMENT_H_INCLUDED
