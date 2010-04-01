#ifndef LABELELEMENT_H_INCLUDED
#define LABELELEMENT_H_INCLUDED


#include "XULWin/Element.h"


namespace XULWin
{

    class LabelElement : public Element
    {
    public:
        static ElementPtr Create(Element * inParent, const AttributesMapping & inAttr)
        { return Element::Create<LabelElement>(inParent, inAttr); }

        static const char * Type() { return "label"; }
    
    private:
        friend class Element;
        LabelElement(Element * inParent, const AttributesMapping & inAttributesMapping);
    };

} // namespace XULWin


#endif // LABELELEMENT_H_INCLUDED
