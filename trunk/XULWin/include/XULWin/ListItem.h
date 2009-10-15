#ifndef LISTITEM_H_INCLUDED
#define LISTITEM_H_INCLUDED


#include "XULWin/Element.h"


namespace XULWin
{

    class ListItem : public Element
    {
    public:
        static ElementPtr Create(Element * inParent, const AttributesMapping & inAttr)
        { return Element::Create<ListItem>(inParent, inAttr); }

        static const char * Type() { return "listitem"; }

        virtual bool init();
    
    private:
        friend class Element;
        ListItem(Element * inParent, const AttributesMapping & inAttributesMapping);
    };

} // namespace XULWin


#endif // LISTITEM_H_INCLUDED
