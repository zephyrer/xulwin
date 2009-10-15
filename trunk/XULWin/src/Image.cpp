#include "XULWin/Image.h"
#include "XULWin/ImageImpl.h"
#include "XULWin/AttributeController.h"
#include "XULWin/Decorator.h"
#include "XULWin/ElementCreationSupport.h"


namespace XULWin
{


    Image::Image(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(Image::Type(),
                inParent,
                new MarginDecorator(CreateNativeControl<ImageImpl>(inParent, inAttributesMapping)))
    {
    }


    bool Image::init()
    {
        return Element::init();
    }


} // namespace XULWin
