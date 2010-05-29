#ifndef SCRIPT_H_INCLUDED
#define SCRIPT_H_INCLUDED


#include "XULWin/Element.h"


namespace XULWin
{

    class XMLScript : public Element
    {
    public:
        static ElementPtr Create(Element * inParent, const AttributesMapping & inAttr)
        {
            return Element::Create<XMLScript>(inParent, inAttr);
        }

        static const char * TagName()
        {
            return "script";
        }

        virtual bool init();

    private:
        friend class Element;
        XMLScript(Element * inParent, const AttributesMapping & inAttributesMapping);
    };

} // namespace XULWin


#endif // SCRIPT_H_INCLUDED
