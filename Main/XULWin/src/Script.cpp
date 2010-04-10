#include "XULWin/ScriptElement.h"
#include "XULWin/AttributeController.h"
#include "XULWin/Decorator.h"


namespace XULWin
{

    class Script : public PassiveComponent
    {
    public:
        typedef PassiveComponent Super;

        Script(Component * inParent, const AttributesMapping & inAttributesMapping);
    };


    ScriptElement::ScriptElement(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(ScriptElement::TagName(),
                inParent,
                new Script(inParent->component(), inAttributesMapping))
    {
    }


    bool ScriptElement::init()
    {
        setAttribute("value", innerText());
        return Element::init();
    }


    Script::Script(Component * inParent, const AttributesMapping & inAttributesMapping) :
        PassiveComponent(inParent, inAttributesMapping)
    {
    }

} // namespace XULWin
