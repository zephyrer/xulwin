#ifndef LISTCOL_H_INCLUDED
#define LISTCOL_H_INCLUDED


#include "XULWin/Element.h"


namespace XULWin
{

    class XMLListCol : public Element
    {
    public:
        static ElementPtr Create(Element * inParent, const AttributesMapping & inAttr)
        {
            return Element::Create<XMLListCol>(inParent, inAttr);
        }

        static const char * TagName()
        {
            return "listcol";
        }

        virtual bool init();

    private:
        friend class Element;
        XMLListCol(Element * inParent, const AttributesMapping & inAttributesMapping);
    };

} // namespace XULWin


#endif // LISTCOL_H_INCLUDED
