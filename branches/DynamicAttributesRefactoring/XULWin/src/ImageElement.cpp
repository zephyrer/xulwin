#include "XULWin/ImageElement.h"
#include "XULWin/Image.h"
#include "XULWin/ComponentFactory.h"
#include "XULWin/AttributeController.h"
#include "XULWin/Decorator.h"
#include "XULWin/ElementCreationSupport.h"


namespace XULWin
{


    ImageElement::ImageElement(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(ImageElement::Type(),
                inParent,
                ComponentFactory::Instance().createComponent<MarginDecorator, Image>(inParent->component(), inAttributesMapping))
    {
    }


} // namespace XULWin
