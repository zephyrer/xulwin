#ifndef MENUBAR_H_INCLUDED
#define MENUBAR_H_INCLUDED


#include "XULWin/Element.h"


namespace XULWin
{

    class XMLMenuBar : public Element
    {
    public:
        static ElementPtr Create(Element * inParent, const AttributesMapping & inAttr)
        {
            return Element::Create<XMLMenuBar>(inParent, inAttr);
        }

        static const char * TagName()
        {
            return "menubar";
        }

    private:
        friend class Element;
        XMLMenuBar(Element * inParent, const AttributesMapping & inAttr);
    };

} // namespace XULWin


#endif // MENUBAR_H_INCLUDED
