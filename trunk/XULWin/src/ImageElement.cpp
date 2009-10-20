#include "XULWin/ImageElement.h"
#include "XULWin/Image.h"
#include "XULWin/AttributeController.h"
#include "XULWin/Decorator.h"
#include "XULWin/ElementCreationSupport.h"


namespace XULWin
{


    ImageElement::ImageElement(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(ImageElement::Type(),
                inParent,
                new MarginDecorator(CreateNativeControl<Image>(inParent, inAttributesMapping)))
    {
    }


    bool ImageElement::init()
    {
        return Element::init();
    }


} // namespace XULWin
