#ifndef SCRIPT_H_INCLUDED
#define SCRIPT_H_INCLUDED


#include "XULWin/Element.h"


namespace XULWin
{

    class ScriptElement : public Element
    {
    public:
        static ElementPtr Create(Element * inParent, const AttributesMapping & inAttr)
        { return Element::Create<ScriptElement>(inParent, inAttr); }

        static const char * Type() { return "script"; }

        virtual bool init();
    
    private:
        friend class Element;
        ScriptElement(Element * inParent, const AttributesMapping & inAttributesMapping);
    };

} // namespace XULWin


#endif // SCRIPT_H_INCLUDED
