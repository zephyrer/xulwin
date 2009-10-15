#ifndef LISTCOL_H_INCLUDED
#define LISTCOL_H_INCLUDED


#include "XULWin/Element.h"


namespace XULWin
{

    class ListCol : public Element
    {
    public:
        static ElementPtr Create(Element * inParent, const AttributesMapping & inAttr)
        { return Element::Create<ListCol>(inParent, inAttr); }

        static const char * Type() { return "listcol"; }

        virtual bool init();
    
    private:
        friend class Element;
        ListCol(Element * inParent, const AttributesMapping & inAttributesMapping);
    };

} // namespace XULWin


#endif // LISTCOL_H_INCLUDED
