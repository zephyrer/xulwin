#ifndef LISTVIEW_H_INCLUDED
#define LISTVIEW_H_INCLUDED


#include "XULWin/Element.h"


namespace XULWin
{

    class ListView : public Element
    {
    public:
        static ElementPtr Create(Element * inParent, const AttributesMapping & inAttr)
        { return Element::Create<ListView>(inParent, inAttr); }

        // XUL uses the "listbox" element both for list boxes and list views.
        // XUL does not have a listview element.
        static const char * Type() { return "listbox"; }

        virtual bool init();
    
    private:
        friend class Element;
        ListView(Element * inParent, const AttributesMapping & inAttributesMapping);
    };

} // namespace XULWin


#endif // LISTVIEW_H_INCLUDED
