#include "XULWin/XMLScript.h"
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


    XMLScript::XMLScript(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(XMLScript::TagName(),
                inParent,
                new Script(inParent->component(), inAttributesMapping))
    {
    }


    bool XMLScript::init()
    {
        setAttribute("value", innerText());
        return Element::init();
    }


    Script::Script(Component * inParent, const AttributesMapping & inAttributesMapping) :
        PhonyComponent(inParent, inAttributesMapping)
    {
    }

} // namespace XULWin
