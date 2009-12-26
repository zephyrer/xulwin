#ifndef LISTCOLS_H_INCLUDED
#define LISTCOLS_H_INCLUDED


#include "XULWin/Element.h"


namespace XULWin
{

    class ListColsElement : public Element
    {
    public:
        static ElementPtr Create(Element * inParent, const AttributesMapping & inAttr)
        { return Element::Create<ListColsElement>(inParent, inAttr); }

        static const char * Type() { return "listcols"; }

        virtual bool init();
    
    private:
        friend class Element;
        ListColsElement(Element * inParent, const AttributesMapping & inAttributesMapping);
    };

} // namespace XULWin


#endif // LISTCOLS_H_INCLUDED
