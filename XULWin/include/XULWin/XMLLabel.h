#ifndef LABELELEMENT_H_INCLUDED
#define LABELELEMENT_H_INCLUDED


#include "XULWin/Element.h"


namespace XULWin
{

    class XMLLabel : public Element
    {
    public:
        static ElementPtr Create(Element * inParent, const AttributesMapping & inAttr)
        {
            return Element::Create<XMLLabel>(inParent, inAttr);
        }

        static const char * TagName()
        {
            return "label";
        }

    private:
        friend class Element;
        XMLLabel(Element * inParent, const AttributesMapping & inAttributesMapping);
    };

} // namespace XULWin


#endif // LABELELEMENT_H_INCLUDED
