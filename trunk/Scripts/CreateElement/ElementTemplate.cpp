#include "XULWin/{{ELEMENT_NAME}}.h"
#include "XULWin/{{ELEMENT_NAME}}Impl.h"
#include "XULWin/AttributeController.h"
#include "XULWin/Decorator.h"


namespace XULWin
{

    {{ELEMENT_NAME}}::{{ELEMENT_NAME}}(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element({{ELEMENT_NAME}}::Type(),
                inParent,
                new {{ELEMENT_NAME}}Impl(inParent->impl(), inAttributesMapping))
    {
    }


    bool {{ELEMENT_NAME}}::init()
    {
        return Element::init();
    }

} // namespace XULWin
