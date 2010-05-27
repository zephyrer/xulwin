#ifndef IMAGE_H_INCLUDED
#define IMAGE_H_INCLUDED


#include "XULWin/Element.h"
#include "XULWin/AttributeController.h"


namespace XULWin
{

    class ImageElement : public Element
    {
    public:
        static ElementPtr Create(Element * inParent, const AttributesMapping & inAttr)
        {
            return Element::Create<ImageElement>(inParent, inAttr);
        }

        static const char * TagName()
        {
            return "image";
        }

    private:
        friend class Element;
        ImageElement(Element * inParent, const AttributesMapping & inAttributesMapping);
    };

} // namespace XULWin


#endif // IMAGE_H_INCLUDED
