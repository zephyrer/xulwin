#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED


#include "XULWin/Element.h"


namespace XULWin
{

    class MenuElement : public Element
    {
    public:
        static ElementPtr Create(Element * inParent, const AttributesMapping & inAttr)
        { return Element::Create<MenuElement>(inParent, inAttr); }

        static const char * Type() { return "menu"; }

    private:
        friend class Element;
        MenuElement(Element * inParent, const AttributesMapping & inAttributesMapping);
    };

} // namespace XULWin


#endif // MENU_H_INCLUDED
