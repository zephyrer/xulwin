#include "XULWin/XMLSVG.h"
#include "XULWin/SVG.h"
#include "XULWin/GdiplusLoader.h"
#include "XULWin/XMLImage.h"
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

    XMLSVG::XMLSVG(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(XMLSVG::TagName(),
                inParent,
                new SVGCanvas(inParent->component(), inAttributesMapping))
    {
    }


    XMLSVGGroup::XMLSVGGroup(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(XMLSVGGroup::TagName(),
                inParent,
                new SVGGroup(inParent->component(), inAttributesMapping))
    {
    }


    XMLSVGPolygon::XMLSVGPolygon(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(XMLSVGPolygon::TagName(),
                inParent,
                new SVGPolygon(inParent->component(), inAttributesMapping))
    {
    }


    XMLSVGRect::XMLSVGRect(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(XMLSVGRect::TagName(),
                inParent,
                new SVGRect(inParent->component(), inAttributesMapping))
    {
    }


    XMLSVGPath::XMLSVGPath(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(XMLSVGPath::TagName(),
                inParent,
                new SVGPath(inParent->component(), inAttributesMapping))
    {
    }


} // namespace XULWin