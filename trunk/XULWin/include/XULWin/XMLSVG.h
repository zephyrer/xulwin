#ifndef SVG_H_INCLUDED
#define SVG_H_INCLUDED


#include "XULWin/Element.h"


namespace XULWin
{

    class XMLSVG : public Element
    {
    public:
        static ElementPtr Create(Element * inParent, const AttributesMapping & inAttr)
        {
            return Element::Create<XMLSVG>(inParent, inAttr);
        }

        static const char * TagName()
        {
            return "svg";
        }

    private:
        friend class Element;
        XMLSVG(Element * inParent, const AttributesMapping & inAttributesMapping);
    };


    class XMLSVGGroup : public Element
    {
    public:
        static ElementPtr Create(Element * inParent, const AttributesMapping & inAttr)
        {
            return Element::Create<XMLSVGGroup>(inParent, inAttr);
        }

        static const char * TagName()
        {
            return "g";
        }

    private:
        friend class Element;
        XMLSVGGroup(Element * inParent, const AttributesMapping & inAttributesMapping);
    };


    class XMLSVGPolygon : public Element
    {
    public:
        static ElementPtr Create(Element * inParent, const AttributesMapping & inAttr)
        {
            return Element::Create<XMLSVGPolygon>(inParent, inAttr);
        }

        static const char * TagName()
        {
            return "polygon";
        }

    private:
        friend class Element;
        XMLSVGPolygon(Element * inParent, const AttributesMapping & inAttributesMapping);
    };


    class XMLSVGRect : public Element
    {
    public:
        static ElementPtr Create(Element * inParent, const AttributesMapping & inAttr)
        {
            return Element::Create<XMLSVGRect>(inParent, inAttr);
        }

        static const char * TagName()
        {
            return "rect";
        }

    private:
        friend class Element;
        XMLSVGRect(Element * inParent, const AttributesMapping & inAttributesMapping);
    };


    class XMLSVGPath : public Element
    {
    public:
        static ElementPtr Create(Element * inParent, const AttributesMapping & inAttr)
        {
            return Element::Create<XMLSVGPath>(inParent, inAttr);
        }

        static const char * TagName()
        {
            return "path";
        }

    private:
        friend class Element;
        XMLSVGPath(Element * inParent, const AttributesMapping & inAttributesMapping);
    };

} // namespace XULWin


#endif // SVG_H_INCLUDED
