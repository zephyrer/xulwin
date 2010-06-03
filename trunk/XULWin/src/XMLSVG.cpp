#include "XULWin/XMLSVG.h"
#include "XULWin/SVG.h"
#include "XULWin/GdiplusLoader.h"
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

    XMLSVG::XMLSVG(Element * inParent, const AttributesMapping & inAttr) :
        Element(XMLSVG::TagName(),
                inParent,
                new SVGCanvas(inParent->component(), inAttr))
    {
    }


    XMLSVGGroup::XMLSVGGroup(Element * inParent, const AttributesMapping & inAttr) :
        Element(XMLSVGGroup::TagName(),
                inParent,
                new SVGGroup(inParent->component(), inAttr))
    {
    }


    XMLSVGPolygon::XMLSVGPolygon(Element * inParent, const AttributesMapping & inAttr) :
        Element(XMLSVGPolygon::TagName(),
                inParent,
                new SVGPolygon(inParent->component(), inAttr))
    {
    }


    XMLSVGRect::XMLSVGRect(Element * inParent, const AttributesMapping & inAttr) :
        Element(XMLSVGRect::TagName(),
                inParent,
                new SVGRect(inParent->component(), inAttr))
    {
    }


    XMLSVGPath::XMLSVGPath(Element * inParent, const AttributesMapping & inAttr) :
        Element(XMLSVGPath::TagName(),
                inParent,
                new SVGPath(inParent->component(), inAttr))
    {
    }


} // namespace XULWin