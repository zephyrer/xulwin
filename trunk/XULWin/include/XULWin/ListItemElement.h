#ifndef LISTITEM_H_INCLUDED
#define LISTITEM_H_INCLUDED


#include "XULWin/Element.h"


namespace XULWin
{

    class ListItemElement : public Element
    {
    public:
        static ElementPtr Create(Element * inParent, const AttributesMapping & inAttr)
        {
            return Element::Create<ListItemElement>(inParent, inAttr);
        }

        static const char * TagName()
        {
            return "listitem";
        }

        virtual bool init();

    private:
        friend class Element;
        ListItemElement(Element * inParent, const AttributesMapping & inAttributesMapping);
    };

} // namespace XULWin


#endif // LISTITEM_H_INCLUDED
