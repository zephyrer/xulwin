#include "XULWin/ImageElement.h"
#include "XULWin/Image.h"
#include "XULWin/ComponentFactory.h"
#include "XULWin/AttributeController.h"
#include "XULWin/Decorator.h"


namespace XULWin
{


    ImageElement::ImageElement(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(ImageElement::TagName(),
                inParent,
                ComponentFactory::Instance().createComponent<MarginDecorator, Image>(inParent->component(), inAttributesMapping))
    {
    }


} // namespace XULWin
