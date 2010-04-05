#ifndef LISTCELL_H_INCLUDED
#define LISTCELL_H_INCLUDED


#include "XULWin/Element.h"


namespace XULWin
{

    class ListCellElement : public Element
    {
    public:
        static ElementPtr Create(Element * inParent, const AttributesMapping & inAttr)
        {
            return Element::Create<ListCellElement>(inParent, inAttr);
        }

        static const char * Type()
        {
            return "listcell";
        }

        virtual bool init();

    private:
        friend class Element;
        ListCellElement(Element * inParent, const AttributesMapping & inAttributesMapping);
    };

} // namespace XULWin


#endif // LISTCELL_H_INCLUDED
