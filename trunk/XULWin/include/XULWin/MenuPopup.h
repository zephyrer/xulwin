#ifndef MENUPOPUP_H_INCLUDED
#define MENUPOPUP_H_INCLUDED


#include "XULWin/Element.h"


namespace XULWin
{

    class MenuPopup : public Element
    {
    public:
        static ElementPtr Create(Element * inParent, const AttributesMapping & inAttr)
        { return Element::Create<MenuPopup>(inParent, inAttr); }

        virtual ~MenuPopup();

        static const char * Type() { return "menupopup"; }

    private:
        friend class Element;
        MenuPopup(Element * inParent, const AttributesMapping & inAttributesMapping);
    };

} // namespace XULWin


#endif // MENUPOPUP_H_INCLUDED
