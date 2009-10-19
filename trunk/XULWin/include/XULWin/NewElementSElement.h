#ifndef NEWELEMENTS_H_INCLUDED
#define NEWELEMENTS_H_INCLUDED


#include "XULWin/Element.h"


namespace XULWin
{

    class NewElementS : public Element
    {
    public:
        static ElementPtr Create(Element * inParent, const AttributesMapping & inAttr)
        { return Element::Create<NewElementS>(inParent, inAttr); }

        static const char * Type() { return "newelements"; }

        virtual bool init();
    
    private:
        friend class Element;
        NewElementS(Element * inParent, const AttributesMapping & inAttributesMapping);
    };

} // namespace XULWin


#endif // NEWELEMENTS_H_INCLUDED
