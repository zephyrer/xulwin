#include "XULWin/SVGElement.h"
#include "XULWin/SVG.h"
#include "XULWin/GdiplusLoader.h"
#include "XULWin/ImageElement.h"
#include "XULWin/AttributeController.h"
#include "XULWin/StyleController.h"
#include "XULWin/Decorator.h"
#include "XULWin/Component.h"
#include "XULWin/Defaults.h"
#include "XULWin/Fallible.h"
#include <boost/bind.hpp>
#include <boost/noncopyable.hpp>
#include <boost/scoped_ptr.hpp>
#include <gdiplus.h>


namespace XULWin
{

    SVGElement::SVGElement(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(SVGElement::Type(),
                inParent,
                new SVGCanvas(inParent->component(), inAttributesMapping))
    {
    }

        
    SVGGroupElement::SVGGroupElement(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(SVGGroupElement::Type(),
                inParent,
                new SVGGroup(inParent->component(), inAttributesMapping))
    {
    }


    SVGPolygonElement::SVGPolygonElement(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(SVGPolygonElement::Type(),
                inParent,
                new SVGPolygon(inParent->component(), inAttributesMapping))
    {
    }


    SVGRectElement::SVGRectElement(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(SVGRectElement::Type(),
                inParent,
                new SVGRect(inParent->component(), inAttributesMapping))
    {
    }


    SVGPathElement::SVGPathElement(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(SVGPathElement::Type(),
                inParent,
                new SVGPath(inParent->component(), inAttributesMapping))
    {
    }


} // namespace XULWin