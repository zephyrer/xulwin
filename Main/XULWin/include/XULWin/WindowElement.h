#ifndef WINDOWELEMENT_H_INCLUDED
#define WINDOWELEMENT_H_INCLUDED


#include "XULWin/Element.h"


namespace XULWin
{


    class WindowElement : public Element
    {
    public:
        static ElementPtr Create(Element * inParent, const AttributesMapping & inAttr)
        { return Element::Create<WindowElement>(inParent, inAttr); }

        static const char * Type() { return "window"; }


        enum Positioning
        {
            DefaultPosition,
            CenterInScreen
        };

        void show(Positioning inPositioning);

        void showModal(Positioning inPositioning);

        void close();

    private:
        friend class Element;
        WindowElement(Element * inParent, const AttributesMapping & inAttributesMapping);
    };


} // namespace XULWin


#endif // WINDOWELEMENT_H_INCLUDED
