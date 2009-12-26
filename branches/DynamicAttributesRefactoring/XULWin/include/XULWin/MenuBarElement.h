#ifndef MENUBAR_H_INCLUDED
#define MENUBAR_H_INCLUDED


#include "XULWin/Element.h"


namespace XULWin
{

    class MenuBarElement : public Element
    {
    public:
        static ElementPtr Create(Element * inParent, const AttributesMapping & inAttr)
        { return Element::Create<MenuBarElement>(inParent, inAttr); }

        static const char * Type() { return "menubar"; }

    private:
        friend class Element;
        MenuBarElement(Element * inParent, const AttributesMapping & inAttributesMapping);
    };

} // namespace XULWin


#endif // MENUBAR_H_INCLUDED
