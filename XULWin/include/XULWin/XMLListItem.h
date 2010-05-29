#ifndef LISTITEM_H_INCLUDED
#define LISTITEM_H_INCLUDED


#include "XULWin/Element.h"


namespace XULWin
{

    class XMLListItem : public Element
    {
    public:
        static ElementPtr Create(Element * inParent, const AttributesMapping & inAttr)
        {
            return Element::Create<XMLListItem>(inParent, inAttr);
        }

        static const char * TagName()
        {
            return "listitem";
        }

        virtual bool init();

    private:
        friend class Element;
        XMLListItem(Element * inParent, const AttributesMapping & inAttributesMapping);
    };

} // namespace XULWin


#endif // LISTITEM_H_INCLUDED
