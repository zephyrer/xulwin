#ifndef LISTHEADER_H_INCLUDED
#define LISTHEADER_H_INCLUDED


#include "XULWin/Element.h"


namespace XULWin
{

    class XMLListHeader : public Element
    {
    public:
        static ElementPtr Create(Element * inParent, const AttributesMapping & inAttr)
        {
            return Element::Create<XMLListHeader>(inParent, inAttr);
        }

        static const char * TagName()
        {
            return "listheader";
        }

        virtual bool init();

    private:
        friend class Element;
        XMLListHeader(Element * inParent, const AttributesMapping & inAttributesMapping);
    };

} // namespace XULWin


#endif // LISTHEADER_H_INCLUDED
