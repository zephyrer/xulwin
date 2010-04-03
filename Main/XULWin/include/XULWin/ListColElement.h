#ifndef LISTCOL_H_INCLUDED
#define LISTCOL_H_INCLUDED


#include "XULWin/Element.h"


namespace XULWin
{

    class ListColElement : public Element
    {
    public:
        static ElementPtr Create(Element * inParent, const AttributesMapping & inAttr)
        {
            return Element::Create<ListColElement>(inParent, inAttr);
        }

        static const char * Type()
        {
            return "listcol";
        }

        virtual bool init();

    private:
        friend class Element;
        ListColElement(Element * inParent, const AttributesMapping & inAttributesMapping);
    };

} // namespace XULWin


#endif // LISTCOL_H_INCLUDED
