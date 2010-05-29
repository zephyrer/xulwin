#ifndef MENUPOPUP_H_INCLUDED
#define MENUPOPUP_H_INCLUDED


#include "XULWin/Element.h"


namespace XULWin
{

    class XMLMenuPopup : public Element
    {
    public:
        static ElementPtr Create(Element * inParent, const AttributesMapping & inAttr)
        {
            return Element::Create<XMLMenuPopup>(inParent, inAttr);
        }

        virtual ~XMLMenuPopup();

        static const char * TagName()
        {
            return "menupopup";
        }

    private:
        friend class Element;
        XMLMenuPopup(Element * inParent, const AttributesMapping & inAttributesMapping);
    };

} // namespace XULWin


#endif // MENUPOPUP_H_INCLUDED
