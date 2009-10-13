#include "XULWin/SVG.h"
#include "XULWin/SVGImpl.h"
#include "XULWin/GdiplusLoader.h"
#include "XULWin/Image.h"
#include "XULWin/AttributeController.h"
#include "XULWin/StyleController.h"
#include "XULWin/Decorator.h"
#include "XULWin/ElementImpl.h"
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


    SVG::SVG(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(SVG::Type(),
                inParent,
                new SVGCanvas(inParent->impl(), inAttributesMapping))
    {
    }

        
    Group::Group(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(Group::Type(),
                inParent,
                new SVGGroupImpl(inParent->impl(), inAttributesMapping))
    {
    }


    Polygon::Polygon(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(Polygon::Type(),
                inParent,
                new SVGPolygonImpl(inParent->impl(), inAttributesMapping))
    {
    }


    SVGRect::SVGRect(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(SVGRect::Type(),
                inParent,
                new SVGRectImpl(inParent->impl(), inAttributesMapping))
    {
    }


    Path::Path(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(Path::Type(),
                inParent,
                new SVGPathImpl(inParent->impl(), inAttributesMapping))
    {
    }



} // namespace SVG

} // namespace XULWin