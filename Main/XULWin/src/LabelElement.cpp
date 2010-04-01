#include "XULWin/LabelElement.h"
#include "XULWin/AttributeController.h"
#include "XULWin/ComponentFactory.h"
#include "XULWin/Decorator.h"
#include "XULWin/Label.h"


namespace XULWin
{

    LabelElement::LabelElement(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(LabelElement::Type(),
                inParent,
                ComponentFactory::Instance().createComponent<MarginDecorator, Label>(inParent->component(), inAttributesMapping))
    {
    }

} // namespace XULWin
