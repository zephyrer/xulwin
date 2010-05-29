#ifndef MENUITEM_H_INCLUDED
#define MENUITEM_H_INCLUDED


#include "XULWin/Element.h"


namespace XULWin
{

    class XMLMenuItem : public Element
    {
    public:
        static ElementPtr Create(Element * inParent, const AttributesMapping & inAttr)
        {
            return Element::Create<XMLMenuItem>(inParent, inAttr);
        }

        static const char * TagName()
        {
            return "menuitem";
        }

        virtual ~XMLMenuItem();

        std::string label() const;

        std::string value() const;

    private:
        friend class Element;
        XMLMenuItem(Element * inParent, const AttributesMapping & inAttr);
    };

} // namespace XULWin


#endif // MENUITEM_H_INCLUDED
