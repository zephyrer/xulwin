#ifndef IMAGE_H_INCLUDED
#define IMAGE_H_INCLUDED


#include "XULWin/Element.h"
#include "XULWin/AttributeController.h"


namespace XULWin
{

    class Image : public Element
    {
    public:
        static ElementPtr Create(Element * inParent, const AttributesMapping & inAttr)
        { return Element::Create<Image>(inParent, inAttr); }

        static const char * Type() { return "image"; }

        virtual bool init();

        std::string value() const;
    
    private:
        friend class Element;
        Image(Element * inParent, const AttributesMapping & inAttributesMapping);
    };

} // namespace XULWin


#endif // IMAGE_H_INCLUDED
