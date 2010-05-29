#ifndef IMAGE_H_INCLUDED
#define IMAGE_H_INCLUDED


#include "XULWin/Element.h"


namespace XULWin
{

    class XMLImage : public Element
    {
    public:
        static ElementPtr Create(Element * inParent, const AttributesMapping & inAttr)
        {
            return Element::Create<XMLImage>(inParent, inAttr);
        }

        static const char * TagName()
        {
            return "image";
        }

    private:
        friend class Element;
        XMLImage(Element * inParent, const AttributesMapping & inAttributesMapping);
    };

} // namespace XULWin


#endif // IMAGE_H_INCLUDED
