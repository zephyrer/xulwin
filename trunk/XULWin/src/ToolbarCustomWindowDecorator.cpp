#include "XULWin/ToolbarCustomWindowDecorator.h"
#include "XULWin/Toolbar.h"
#include "XULWin/WindowsToolbar.h"


namespace XULWin
{

    ToolbarCustomWindowDecorator::ToolbarCustomWindowDecorator(Component * inDecoratedElement, boost::weak_ptr<Windows::Toolbar> inToolbar) :
        Decorator(inDecoratedElement),
        IECustomWindow(),
        mCustomWindow(0)
    {
        if (NativeComponent * native = mDecoratedElement->downcast<NativeComponent>())
        {
            mCustomWindow = new ConcreteCustomWindow(inToolbar, mDecoratedElement->componentId(), native->handle());
        }
    }


    UInt32 ToolbarCustomWindowDecorator::componentId() const
    {
        if (mCustomWindow)
        {
            mCustomWindow->componentId();
        }
        return 0;
    }


    bool ToolbarCustomWindowDecorator::hasFocus() const
    {
        if (mCustomWindow)
        {
            mCustomWindow->hasFocus();
        }
        return false;
    }


    void ToolbarCustomWindowDecorator::setFocus() const
    {
        if (mCustomWindow)
        {
            mCustomWindow->setFocus();
        }
    }


    HWND ToolbarCustomWindowDecorator::handle() const
    {
        if (mCustomWindow)
        {
            return mCustomWindow->handle();
        }
        return 0;
    }


    ToolbarCustomWindowDecorator::ConcreteCustomWindow::ConcreteCustomWindow(boost::weak_ptr<Windows::Toolbar> inToolbar, UInt32 inComponentId, HWND inHandle) :
        mToolbar(inToolbar),
        mHandle(inHandle),
        mComponentId(inComponentId)
    {
    }


    bool ToolbarCustomWindowDecorator::ConcreteCustomWindow::hasFocus() const
    {
        return ::GetFocus() == mHandle;
    }


    void ToolbarCustomWindowDecorator::ConcreteCustomWindow::setFocus() const
    {
        ::SetFocus(mHandle);
    }


    HWND ToolbarCustomWindowDecorator::ConcreteCustomWindow::handle() const
    {
        return mHandle;
    }


    UInt32 ToolbarCustomWindowDecorator::ConcreteCustomWindow::componentId() const
    {
        return mComponentId;
    }


    bool ToolbarCustomWindowDecorator::init()
    {
        if (mCustomWindow)
        {
            if (XULWin::Toolbar * toolbar = parent()->downcast<XULWin::Toolbar>())
            {
                mDecoratedElement->move(0, 0, mDecoratedElement->getWidth(), mDecoratedElement->getHeight());
                mDecoratedElement->rebuildLayout();
                toolbar->nativeToolbar()->add(mCustomWindow);
            }
        }
        return mDecoratedElement->init();
    }

} // namespace XULWin
