#ifndef WINDOWELEMENT_H_INCLUDED
#define WINDOWELEMENT_H_INCLUDED


#include "XULWin/Element.h"
#include "XULWin/Enums.h"


namespace XULWin
{


    class WindowElement : public Element
    {
    public:
        static ElementPtr Create(Element * inParent, const AttributesMapping & inAttr)
        {
            return Element::Create<WindowElement>(inParent, inAttr);
        }

        static const char * TagName()
        {
            return "window";
        }

        void show(WindowPos inWindowPos);

        void showModal(WindowPos inWindowPos);

        void close();

    private:
        friend class Element;
        WindowElement(Element * inParent, const AttributesMapping & inAttributesMapping);
    };


} // namespace XULWin


#endif // WINDOWELEMENT_H_INCLUDED
