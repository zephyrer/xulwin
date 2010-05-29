#ifndef LISTCELL_H_INCLUDED
#define LISTCELL_H_INCLUDED


#include "XULWin/Element.h"


namespace XULWin
{

    class XMLListCell : public Element
    {
    public:
        static ElementPtr Create(Element * inParent, const AttributesMapping & inAttr)
        {
            return Element::Create<XMLListCell>(inParent, inAttr);
        }

        static const char * TagName()
        {
            return "listcell";
        }

        virtual bool init();

    private:
        friend class Element;
        XMLListCell(Element * inParent, const AttributesMapping & inAttributesMapping);
    };

} // namespace XULWin


#endif // LISTCELL_H_INCLUDED
