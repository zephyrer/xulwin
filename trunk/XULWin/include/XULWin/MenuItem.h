#ifndef MENUITEM_H_INCLUDED
#define MENUITEM_H_INCLUDED


#include "XULWin/Element.h"


namespace XULWin
{

    class MenuItem : public Element
    {
    public:
        static ElementPtr Create(Element * inParent, const AttributesMapping & inAttr)
        { return Element::Create<MenuItem>(inParent, inAttr); }

        static const char * Type() { return "menuitem"; }

        virtual ~MenuItem();

        std::string label() const;

        std::string value() const;

    private:
        friend class Element;
        MenuItem(Element * inParent, const AttributesMapping & inAttributesMapping);
    };

} // namespace XULWin


#endif // MENUITEM_H_INCLUDED
