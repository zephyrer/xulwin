#include "XULWin/XMLWindow.h"
#include "XULWin/Window.h"
#include "XULWin/ComponentFactory.h"
#include "XULWin/Decorator.h"


namespace XULWin
{


    XMLWindow::XMLWindow(Element * inParent, const AttributesMapping & inAttr) :
        Element(XMLWindow::TagName(),
                inParent,
                new Window(inAttr))
    {
    }


    void XMLWindow::show(WindowPos inPositioning)
    {
        if (Window * nativeWindow = component()->downcast<Window>())
        {
            nativeWindow->show(inPositioning);
        }
    }


    void XMLWindow::showModal(WindowPos inPositioning)
    {
        if (Window * nativeWindow = component()->downcast<Window>())
        {
            nativeWindow->showModal(inPositioning);
        }
    }


    void XMLWindow::close()
    {
        if (Window * nativeWindow = component()->downcast<Window>())
        {
            nativeWindow->close();
        }
    }


} // namespace XULWin
