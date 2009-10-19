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

namespace SVG
{


    SVGElement::SVGElement(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(SVGElement::Type(),
                inParent,
                new SVGCanvas(inParent->component(), inAttributesMapping))
    {
    }

        
    Group::Group(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(Group::Type(),
                inParent,
                new SVGGroupImpl(inParent->component(), inAttributesMapping))
    {
    }


    Polygon::Polygon(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(Polygon::Type(),
                inParent,
                new SVGPolygonImpl(inParent->component(), inAttributesMapping))
    {
    }


    SVGRect::SVGRect(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(SVGRect::Type(),
                inParent,
                new SVGRectImpl(inParent->component(), inAttributesMapping))
    {
    }


    Path::Path(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(Path::Type(),
                inParent,
                new SVGPathImpl(inParent->component(), inAttributesMapping))
    {
    }



} // namespace SVG

} // namespace XULWin