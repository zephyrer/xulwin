#include "XULWin/SVGElement.h"
#include "XULWin/SVG.h"
#include "XULWin/GdiplusLoader.h"
#include "XULWin/ImageElement.h"
#include "XULWin/Decorator.h"
#include "XULWin/Component.h"
#include "XULWin/Defaults.h"
#include "XULWin/Fallible.h"
#include "XULWin/Gdiplus.h"
#include <boost/bind.hpp>
#include <boost/noncopyable.hpp>
#include <boost/scoped_ptr.hpp>


namespace XULWin
{

    SVGElement::SVGElement(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(SVGElement::TagName(),
                inParent,
                new SVGCanvas(inParent->component(), inAttributesMapping))
    {
    }


    SVGGroupElement::SVGGroupElement(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(SVGGroupElement::TagName(),
                inParent,
                new SVGGroup(inParent->component(), inAttributesMapping))
    {
    }


    SVGPolygonElement::SVGPolygonElement(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(SVGPolygonElement::TagName(),
                inParent,
                new SVGPolygon(inParent->component(), inAttributesMapping))
    {
    }


    SVGRectElement::SVGRectElement(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(SVGRectElement::TagName(),
                inParent,
                new SVGRect(inParent->component(), inAttributesMapping))
    {
    }


    SVGPathElement::SVGPathElement(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(SVGPathElement::TagName(),
                inParent,
                new SVGPath(inParent->component(), inAttributesMapping))
    {
    }


} // namespace XULWin