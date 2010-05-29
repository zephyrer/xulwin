#ifndef LISTHEAD_H_INCLUDED
#define LISTHEAD_H_INCLUDED


#include "XULWin/Element.h"


namespace XULWin
{

    class XMLListHead : public Element
    {
    public:
        static ElementPtr Create(Element * inParent, const AttributesMapping & inAttr)
        {
            return Element::Create<XMLListHead>(inParent, inAttr);
        }

        static const char * TagName()
        {
            return "listhead";
        }

        virtual bool init();

    private:
        friend class Element;
        XMLListHead(Element * inParent, const AttributesMapping & inAttributesMapping);
    };

} // namespace XULWin


#endif // LISTHEAD_H_INCLUDED
