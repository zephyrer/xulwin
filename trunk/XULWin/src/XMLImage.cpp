#include "XULWin/XMLImage.h"
#include "XULWin/Image.h"
#include "XULWin/ComponentFactory.h"
#include "XULWin/MarginDecorator.h"
#include "XULWin/Decorator.h"


namespace XULWin
{


    XMLImage::XMLImage(Element * inParent, const AttributesMapping & inAttr) :
        Element(XMLImage::TagName(),
                inParent,
                ComponentFactory::Instance().createComponent<MarginDecorator, Image>(inParent->component(), inAttr))
    {
    }


} // namespace XULWin
