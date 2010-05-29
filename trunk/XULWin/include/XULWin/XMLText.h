#ifndef TEXTELEMENT_H_INCLUDED
#define TEXTELEMENT_H_INCLUDED


#include "XULWin/Element.h"
#include "XULWin/Label.h"


namespace XULWin
{

    // XMLText is the same as label
    class XMLText : public Element
    {
    public:
        static ElementPtr Create(Element * inParent, const AttributesMapping & inAttr)
        {
            return Element::Create<XMLText>(inParent, inAttr);
        }

        static const char * TagName()
        {
            return "text";
        }

    private:
        friend class Element;
        XMLText(Element * inParent, const AttributesMapping & inAttributesMapping);
    };


} // namespace XULWin


#endif // TEXTELEMENT_H_INCLUDED
