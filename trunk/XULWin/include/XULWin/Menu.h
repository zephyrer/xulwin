#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED


#include "XULWin/Element.h"


namespace XULWin
{

    class Menu : public Element
    {
    public:
        static ElementPtr Create(Element * inParent, const AttributesMapping & inAttr)
        { return Element::Create<Menu>(inParent, inAttr); }

        static const char * Type() { return "menu"; }

    private:
        friend class Element;
        Menu(Element * inParent, const AttributesMapping & inAttributesMapping);
    };

} // namespace XULWin


#endif // MENU_H_INCLUDED
