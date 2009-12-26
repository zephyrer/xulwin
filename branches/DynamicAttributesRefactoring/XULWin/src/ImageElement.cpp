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
                new MarginDecorator(CreateControl<Image>(inParent, inAttributesMapping)))
    {
    }


} // namespace XULWin
