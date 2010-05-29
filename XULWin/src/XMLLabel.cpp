#include "XULWin/XMLLabel.h"
#include "XULWin/ComponentFactory.h"
#include "XULWin/Decorator.h"
#include "XULWin/Label.h"


namespace XULWin
{

    XMLLabel::XMLLabel(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(XMLLabel::TagName(),
                inParent,
                ComponentFactory::Instance().createComponent<MarginDecorator, Label>(inParent->component(), inAttributesMapping))
    {
    }

} // namespace XULWin
