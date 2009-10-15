#ifndef LISTCOLS_H_INCLUDED
#define LISTCOLS_H_INCLUDED


#include "XULWin/Element.h"


namespace XULWin
{

    class ListCols : public Element
    {
    public:
        static ElementPtr Create(Element * inParent, const AttributesMapping & inAttr)
        { return Element::Create<ListCols>(inParent, inAttr); }

        static const char * Type() { return "listcols"; }

        virtual bool init();
    
    private:
        friend class Element;
        ListCols(Element * inParent, const AttributesMapping & inAttributesMapping);
    };

} // namespace XULWin


#endif // LISTCOLS_H_INCLUDED
