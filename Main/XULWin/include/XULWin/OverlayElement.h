#ifndef OVERLAYELEMENT_H_INCLUDED
#define OVERLAYELEMENT_H_INCLUDED


#include "XULWin/Component.h"
#include <string>


namespace XULWin
{

    class OverlayElement : public Element
    {
    public:
        static ElementPtr Create(Element * inParent, const AttributesMapping & inAttr)
        { return Element::Create<OverlayElement>(inParent, inAttr); }

        OverlayElement(Element * inParent, const AttributesMapping & inAttributesMapping);

        static const char * Type() { return "overlay"; }
    };

} // namespace XULWin


#endif // OVERLAYELEMENT_H_INCLUDED
