#ifndef SVG_H_INCLUDED
#define SVG_H_INCLUDED


#include "XULWin/Element.h"


namespace XULWin
{

    class SVGElement : public Element
    {
    public:
        static ElementPtr Create(Element * inParent, const AttributesMapping & inAttr)
        {
            return Element::Create<SVGElement>(inParent, inAttr);
        }

        static const char * TagName()
        {
            return "svg";
        }

    private:
        friend class Element;
        SVGElement(Element * inParent, const AttributesMapping & inAttributesMapping);
    };


    class SVGGroupElement : public Element
    {
    public:
        static ElementPtr Create(Element * inParent, const AttributesMapping & inAttr)
        {
            return Element::Create<SVGGroupElement>(inParent, inAttr);
        }

        static const char * TagName()
        {
            return "g";
        }

    private:
        friend class Element;
        SVGGroupElement(Element * inParent, const AttributesMapping & inAttributesMapping);
    };


    class SVGPolygonElement : public Element
    {
    public:
        static ElementPtr Create(Element * inParent, const AttributesMapping & inAttr)
        {
            return Element::Create<SVGPolygonElement>(inParent, inAttr);
        }

        static const char * TagName()
        {
            return "polygon";
        }

    private:
        friend class Element;
        SVGPolygonElement(Element * inParent, const AttributesMapping & inAttributesMapping);
    };


    class SVGRectElement : public Element
    {
    public:
        static ElementPtr Create(Element * inParent, const AttributesMapping & inAttr)
        {
            return Element::Create<SVGRectElement>(inParent, inAttr);
        }

        static const char * TagName()
        {
            return "rect";
        }

    private:
        friend class Element;
        SVGRectElement(Element * inParent, const AttributesMapping & inAttributesMapping);
    };


    class SVGPathElement : public Element
    {
    public:
        static ElementPtr Create(Element * inParent, const AttributesMapping & inAttr)
        {
            return Element::Create<SVGPathElement>(inParent, inAttr);
        }

        static const char * TagName()
        {
            return "path";
        }

    private:
        friend class Element;
        SVGPathElement(Element * inParent, const AttributesMapping & inAttributesMapping);
    };

} // namespace XULWin


#endif // SVG_H_INCLUDED
