#include "XULWin/XMLText.h"
#include "XULWin/ComponentFactory.h"
#include "XULWin/Decorator.h"
#include "XULWin/Text.h"


namespace XULWin
{

    XMLText::XMLText(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(XMLText::TagName(),
                inParent,
                ComponentFactory::Instance().createComponent<MarginDecorator, Label>(inParent->component(), inAttributesMapping))
    {
    }

} // namespace XULWin
