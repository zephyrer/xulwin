#ifndef TEXTELEMENT_H_INCLUDED
#define TEXTELEMENT_H_INCLUDED


#include "XULWin/Element.h"
#include "XULWin/Label.h"


namespace XULWin
{

    // TextElement is the same as label
    class TextElement : public Element
    {
    public:
        static ElementPtr Create(Element * inParent, const AttributesMapping & inAttr)
        {
            return Element::Create<TextElement>(inParent, inAttr);
        }

        static const char * Type()
        {
            return "text";
        }

    private:
        friend class Element;
        TextElement(Element * inParent, const AttributesMapping & inAttributesMapping);
    };


} // namespace XULWin


#endif // TEXTELEMENT_H_INCLUDED
