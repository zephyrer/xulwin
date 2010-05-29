#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED


#include "XULWin/Element.h"


namespace XULWin
{

    class XMLMenu : public Element
    {
    public:
        static ElementPtr Create(Element * inParent, const AttributesMapping & inAttr)
        {
            return Element::Create<XMLMenu>(inParent, inAttr);
        }

        static const char * TagName()
        {
            return "menu";
        }

    private:
        friend class Element;
        XMLMenu(Element * inParent, const AttributesMapping & inAttr);
    };

} // namespace XULWin


#endif // MENU_H_INCLUDED
