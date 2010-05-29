#ifndef OVERLAYELEMENT_H_INCLUDED
#define OVERLAYELEMENT_H_INCLUDED


#include "XULWin/Element.h"
#include <string>


namespace XULWin
{

    class XMLOverlay : public Element
    {
    public:
        static ElementPtr Create(Element * inParent, const AttributesMapping & inAttr)
        {
            return Element::Create<XMLOverlay>(inParent, inAttr);
        }

        XMLOverlay(Element * inParent, const AttributesMapping & inAttributesMapping);

        static const char * TagName()
        {
            return "overlay";
        }
    };

} // namespace XULWin


#endif // OVERLAYELEMENT_H_INCLUDED
