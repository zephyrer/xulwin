#ifndef MENUBAR_H_INCLUDED
#define MENUBAR_H_INCLUDED


#include "XULWin/Element.h"


namespace XULWin
{

    class MenuBar : public Element
    {
    public:
        static ElementPtr Create(Element * inParent, const AttributesMapping & inAttr)
        { return Element::Create<MenuBar>(inParent, inAttr); }

        static const char * Type() { return "menubar"; }

    private:
        friend class Element;
        MenuBar(Element * inParent, const AttributesMapping & inAttributesMapping);
    };

} // namespace XULWin


#endif // MENUBAR_H_INCLUDED
