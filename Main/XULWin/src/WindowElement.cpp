#include "XULWin/WindowElement.h"
#include "XULWin/Window.h"
#include "XULWin/AttributeController.h"
#include "XULWin/ComponentFactory.h"
#include "XULWin/Decorator.h"


namespace XULWin
{


    WindowElement::WindowElement(Element * inParent, const AttributesMapping & inAttributesMapping) :
        Element(WindowElement::Type(),
                inParent,
                new Window(inAttributesMapping))
    {
    }


    void WindowElement::show(Positioning inPositioning)
    {
        if (Window * nativeWindow = component()->downcast<Window>())
        {
            nativeWindow->show(inPositioning);
        }
    }


    void WindowElement::showModal(Positioning inPositioning)
    {
        if (Window * nativeWindow = component()->downcast<Window>())
        {
            nativeWindow->showModal(inPositioning);
        }
    }


    void WindowElement::close()
    {
        if (Window * nativeWindow = component()->downcast<Window>())
        {
            nativeWindow->close();
        }
    }


} // namespace XULWin
