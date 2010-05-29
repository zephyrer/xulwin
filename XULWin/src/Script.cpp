#include "XULWin/ScriptElement.h"
#include "XULWin/Decorator.h"
#include "XULWin/PhonyComponent.h"


namespace XULWin
{

    class Script : public PhonyComponent
    {
    public:
        typedef PhonyComponent Super;

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
        PhonyComponent(inParent, inAttributesMapping)
    {
    }

} // namespace XULWin
