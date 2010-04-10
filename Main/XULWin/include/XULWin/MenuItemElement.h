#ifndef MENUITEM_H_INCLUDED
#define MENUITEM_H_INCLUDED


#include "XULWin/Element.h"


namespace XULWin
{

    class MenuItemElement : public Element
    {
    public:
        static ElementPtr Create(Element * inParent, const AttributesMapping & inAttr)
        {
            return Element::Create<MenuItemElement>(inParent, inAttr);
        }

        static const char * TagName()
        {
            return "menuitem";
        }

        virtual ~MenuItemElement();

        std::string label() const;

        std::string value() const;

    private:
        friend class Element;
        MenuItemElement(Element * inParent, const AttributesMapping & inAttributesMapping);
    };

} // namespace XULWin


#endif // MENUITEM_H_INCLUDED
