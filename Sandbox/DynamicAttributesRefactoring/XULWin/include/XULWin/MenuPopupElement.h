#ifndef MENUPOPUP_H_INCLUDED
#define MENUPOPUP_H_INCLUDED


#include "XULWin/Element.h"


namespace XULWin
{

    class MenuPopupElement : public Element
    {
    public:
        static ElementPtr Create(Element * inParent, const AttributesMapping & inAttr)
        { return Element::Create<MenuPopupElement>(inParent, inAttr); }

        virtual ~MenuPopupElement();

        static const char * Type() { return "menupopup"; }

    private:
        friend class Element;
        MenuPopupElement(Element * inParent, const AttributesMapping & inAttributesMapping);
    };

} // namespace XULWin


#endif // MENUPOPUP_H_INCLUDED
