#ifndef LISTHEAD_H_INCLUDED
#define LISTHEAD_H_INCLUDED


#include "XULWin/Element.h"


namespace XULWin
{

    class ListHeadElement : public Element
    {
    public:
        static ElementPtr Create(Element * inParent, const AttributesMapping & inAttr)
        {
            return Element::Create<ListHeadElement>(inParent, inAttr);
        }

        static const char * Type()
        {
            return "listhead";
        }

        virtual bool init();

    private:
        friend class Element;
        ListHeadElement(Element * inParent, const AttributesMapping & inAttributesMapping);
    };

} // namespace XULWin


#endif // LISTHEAD_H_INCLUDED
