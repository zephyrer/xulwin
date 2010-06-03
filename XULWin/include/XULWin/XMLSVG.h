#ifndef XMLSVG_H_INCLUDED
#define XMLSVG_H_INCLUDED


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
        XMLSVG(Element * inParent, const AttributesMapping & inAttr);
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
        XMLSVGGroup(Element * inParent, const AttributesMapping & inAttr);
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
        XMLSVGPolygon(Element * inParent, const AttributesMapping & inAttr);
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
        XMLSVGRect(Element * inParent, const AttributesMapping & inAttr);
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
        XMLSVGPath(Element * inParent, const AttributesMapping & inAttr);
    };

} // namespace XULWin


#endif // XMLSVG_H_INCLUDED
