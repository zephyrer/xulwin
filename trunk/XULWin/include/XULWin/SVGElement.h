#ifndef SVG_H_INCLUDED
#define SVG_H_INCLUDED


#include "XULWin/Element.h"
#include "XULWin/AttributeController.h"


namespace XULWin
{

namespace SVG
{

    class SVGElement : public Element
    {
    public:
        static ElementPtr Create(Element * inParent, const AttributesMapping & inAttr)
        { return Element::Create<SVGElement>(inParent, inAttr); }

        static const char * Type() { return "svg"; }
    
    private:
        friend class Element;
        SVGElement(Element * inParent, const AttributesMapping & inAttributesMapping);
    };


    class Group : public Element
    {
    public:
        static ElementPtr Create(Element * inParent, const AttributesMapping & inAttr)
        { return Element::Create<Group>(inParent, inAttr); }

        static const char * Type() { return "g"; }
    
    private:
        friend class Element;
        Group(Element * inParent, const AttributesMapping & inAttributesMapping);
    };


    class Polygon : public Element
    {
    public:
        static ElementPtr Create(Element * inParent, const AttributesMapping & inAttr)
        { return Element::Create<Polygon>(inParent, inAttr); }

        static const char * Type() { return "polygon"; }
    
    private:
        friend class Element;
        Polygon(Element * inParent, const AttributesMapping & inAttributesMapping);
    };


    class SVGRect : public Element
    {
    public:
        static ElementPtr Create(Element * inParent, const AttributesMapping & inAttr)
        { return Element::Create<SVGRect>(inParent, inAttr); }

        static const char * Type() { return "rect"; }
    
    private:
        friend class Element;
        SVGRect(Element * inParent, const AttributesMapping & inAttributesMapping);
    };


    class Path : public Element
    {
    public:
        static ElementPtr Create(Element * inParent, const AttributesMapping & inAttr)
        { return Element::Create<Path>(inParent, inAttr); }

        static const char * Type() { return "path"; }
    
    private:
        friend class Element;
        Path(Element * inParent, const AttributesMapping & inAttributesMapping);
    };

} // namespace SVG

} // namespace XULWin


#endif // SVG_H_INCLUDED
