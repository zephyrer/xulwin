#include "XULWin/Script.h"
#include "XULWin/AttributeController.h"
#include "XULWin/Decorator.h"


namespace XULWin
{

    class NativeScript : public PassiveComponent
    {
    public:
        typedef PassiveComponent Super;

        NativeScript(Component * inParent, const AttributesMapping & inAttributesMapping);
    };


    Script::Script(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(Script::Type(),
                inParent,
                new NativeScript(inParent->impl(), inAttributesMapping))
    {
    }


    bool Script::init()
    {
        setAttribute("value", innerText());
        return Element::init();
    }

    
    NativeScript::NativeScript(Component * inParent, const AttributesMapping & inAttributesMapping) :
        PassiveComponent(inParent, inAttributesMapping)
    {
    }

} // namespace XULWin
