#ifndef LISTHEAD_H_INCLUDED
#define LISTHEAD_H_INCLUDED


#include "XULWin/Element.h"


namespace XULWin
{

    class ListHead : public Element
    {
    public:
        static ElementPtr Create(Element * inParent, const AttributesMapping & inAttr)
        { return Element::Create<ListHead>(inParent, inAttr); }

        static const char * Type() { return "listhead"; }

        virtual bool init();
    
    private:
        friend class Element;
        ListHead(Element * inParent, const AttributesMapping & inAttributesMapping);
    };

} // namespace XULWin


#endif // LISTHEAD_H_INCLUDED
