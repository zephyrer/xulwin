#ifndef LISTBOX_H_INCLUDED
#define LISTBOX_H_INCLUDED


#include "XULWin/Element.h"


namespace XULWin
{

    class ListBoxElement : public Element
    {
    public:
        static ElementPtr Create(Element * inParent, const AttributesMapping & inAttr)
        {
            return Element::Create<ListBoxElement>(inParent, inAttr);
        }

        static const char * Type()
        {
            return "listbox";
        }

        virtual bool init();

        virtual void addChild(ElementPtr inChild);

    private:
        void set(const std::string & inType);

        friend class Element;
        ListBoxElement(Element * inParent, const AttributesMapping & inAttributesMapping);
    };

} // namespace XULWin


#endif // LISTBOX_H_INCLUDED
