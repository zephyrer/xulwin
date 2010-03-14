#include "XULWin/OverlayElement.h"
#include "XULWin/Overlay.h"


namespace XULWin
{

    OverlayElement::OverlayElement(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(OverlayElement::Type(),
                inParent,
                new Overlay(0, inAttributesMapping))
    {
    }

} // namespace XULWin
