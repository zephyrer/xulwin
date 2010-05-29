#include "XULWin/XMLOverlay.h"
#include "XULWin/Overlay.h"


namespace XULWin
{

    XMLOverlay::XMLOverlay(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(XMLOverlay::TagName(),
                inParent,
                new Overlay(0, inAttributesMapping))
    {
    }

} // namespace XULWin
