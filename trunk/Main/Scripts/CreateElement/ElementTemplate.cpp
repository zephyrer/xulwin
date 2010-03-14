#include "XULWin/{{ELEMENT_NAME}}.h"
#include "XULWin/{{COMPONENT_NAME}}.h"
#include "XULWin/AttributeController.h"
#include "XULWin/Decorator.h"


namespace XULWin
{

    {{ELEMENT_NAME}}::{{ELEMENT_NAME}}(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element({{ELEMENT_NAME}}::Type(),
                inParent,
                new {{COMPONENT_NAME}}(inParent->component(), inAttributesMapping))
    {
    }

} // namespace XULWin
