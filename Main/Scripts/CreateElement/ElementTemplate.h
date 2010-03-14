#ifndef {{ELEMENT_NAME_UPPER}}_H_INCLUDED
#define {{ELEMENT_NAME_UPPER}}_H_INCLUDED


#include "XULWin/Element.h"


namespace XULWin
{

    class {{ELEMENT_NAME}} : public Element
    {
    public:
        static ElementPtr Create(Element * inParent, const AttributesMapping & inAttr)
        { return Element::Create<{{ELEMENT_NAME}}>(inParent, inAttr); }

        static const char * Type() { return "{{ELEMENT_TYPE}}"; }
    
    private:
        friend class Element;
        {{ELEMENT_NAME}}(Element * inParent, const AttributesMapping & inAttributesMapping);
    };

} // namespace XULWin


#endif // {{ELEMENT_NAME_UPPER}}_H_INCLUDED
