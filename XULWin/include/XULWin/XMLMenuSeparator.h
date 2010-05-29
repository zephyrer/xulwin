#ifndef MENUSEPARATORELEMENT_H_INCLUDED
#define MENUSEPARATORELEMENT_H_INCLUDED


#include "XULWin/Element.h"


namespace XULWin
{

    class XMLMenuSeparator : public Element
    {
    public:
        static ElementPtr Create(Element * inParent, const AttributesMapping & inAttr)
        {
            return Element::Create<XMLMenuSeparator>(inParent, inAttr);
        }

        static const char * TagName()
        {
            return "menuseparator";
        }

    private:
        friend class Element;
        XMLMenuSeparator(Element * inParent, const AttributesMapping & inAttributesMapping);
    };

} // namespace XULWin


#endif // MENUSEPARATORELEMENT_H_INCLUDED
