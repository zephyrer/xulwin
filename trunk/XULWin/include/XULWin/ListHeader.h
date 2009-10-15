#ifndef LISTHEADER_H_INCLUDED
#define LISTHEADER_H_INCLUDED


#include "XULWin/Element.h"


namespace XULWin
{

    class ListHeader : public Element
    {
    public:
        static ElementPtr Create(Element * inParent, const AttributesMapping & inAttr)
        { return Element::Create<ListHeader>(inParent, inAttr); }

        static const char * Type() { return "listheader"; }

        virtual bool init();
    
    private:
        friend class Element;
        ListHeader(Element * inParent, const AttributesMapping & inAttributesMapping);
    };

} // namespace XULWin


#endif // LISTHEADER_H_INCLUDED
