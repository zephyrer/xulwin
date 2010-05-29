#include "XULWin/XMLScript.h"
#include "XULWin/Decorator.h"
#include "XULWin/PhonyComponent.h"


namespace XULWin
{

    class Script : public PhonyComponent 
    {
    public:
        typedef PhonyComponent Super;

        Script(Component * inParent, const AttributesMapping & inAttr);
    };


    XMLScript::XMLScript(Element * inParent, const AttributesMapping & inAttr) :
        Element(XMLScript::TagName(),
                inParent,
                new Script(inParent->component(), inAttr))
    {
    }


    bool XMLScript::init()
    {
        setAttribute("value", innerText());
        return Element::init();
    }


    Script::Script(Component * inParent, const AttributesMapping & inAttr) :
        PhonyComponent(inParent, inAttr)
    {
    }

} // namespace XULWin
