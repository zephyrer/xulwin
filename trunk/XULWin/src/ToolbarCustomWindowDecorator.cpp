#include "XULWin/ToolbarCustomWindowDecorator.h"
#include "XULWin/Toolbar.h"
#include "XULWin/WindowsToolbar.h"


namespace XULWin
{

    ToolbarCustomWindowDecorator::ToolbarCustomWindowDecorator(Component * inDecoratedComponent, boost::weak_ptr<Windows::Toolbar> inToolbar) :
        Decorator(inDecoratedComponent),
        IECustomWindow(),
        mCustomWindow(0) 
    {
        if (NativeComponent * native = mDecoratedComponent->downcast<NativeComponent>())
        {
            mCustomWindow = new ConcreteCustomWindow(inToolbar, mDecoratedComponent->componentId(), native->handle());
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
                mDecoratedComponent->move(0, 0, mDecoratedComponent->getWidth(), mDecoratedComponent->getHeight());
                mDecoratedComponent->rebuildLayout();
                toolbar->nativeToolbar()->add(mCustomWindow);
            }
        }
        return mDecoratedComponent->init();
    }

} // namespace XULWin
