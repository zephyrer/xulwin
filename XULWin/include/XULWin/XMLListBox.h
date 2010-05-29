#ifndef LISTBOX_H_INCLUDED
#define LISTBOX_H_INCLUDED


#include "XULWin/Element.h"


namespace XULWin
{

    class XMLListBox : public Element
    {
    public:
        static ElementPtr Create(Element * inParent, const AttributesMapping & inAttr)
        {
            return Element::Create<XMLListBox>(inParent, inAttr);
        }

        static const char * TagName()
        {
            return "listbox";
        }

        virtual bool init();

        virtual void addChild(ElementPtr inChild);

    private:
        void set(const std::string & inType);

        friend class Element;
        XMLListBox(Element * inParent, const AttributesMapping & inAttributesMapping);
    };

} // namespace XULWin


#endif // LISTBOX_H_INCLUDED
