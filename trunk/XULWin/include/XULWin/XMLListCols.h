#ifndef LISTCOLS_H_INCLUDED
#define LISTCOLS_H_INCLUDED


#include "XULWin/Element.h"


namespace XULWin
{

    class XMLListCols : public Element
    {
    public:
        static ElementPtr Create(Element * inParent, const AttributesMapping & inAttr)
        {
            return Element::Create<XMLListCols>(inParent, inAttr);
        }

        static const char * TagName()
        {
            return "listcols";
        }

        virtual bool init();

    private:
        friend class Element;
        XMLListCols(Element * inParent, const AttributesMapping & inAttr);
    };

} // namespace XULWin


#endif // LISTCOLS_H_INCLUDED
