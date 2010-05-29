#include "XULWin/TextElement.h"
#include "XULWin/ComponentFactory.h"
#include "XULWin/Decorator.h"
#include "XULWin/Text.h"


namespace XULWin
{

    TextElement::TextElement(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(TextElement::TagName(),
                inParent,
                ComponentFactory::Instance().createComponent<MarginDecorator, Label>(inParent->component(), inAttributesMapping))
    {
    }

} // namespace XULWin
