#include "XULWin/Toolbar/Toolbar.h"


namespace XULWin
{



    ///**
    // * In the WinAPI toolbar commands are sent from the toolbar window to its parent window.
    // * We need to find out which toolbar button sent the message, and then
    // * find the corresponding ToolbarButton element object.
    // * Once we found this object we can look for any associated callbacks.
    // */
    //bool ScopedEventListener::handleToolbarCommand(MsgId inMessageId,
    //                                               WPARAM wParam,
    //                                               LPARAM lParam,
    //                                               LRESULT & ret)
    //{
    //    XULWin::Toolbar * toolbar = inMessageId.element()->component()->downcast<XULWin::Toolbar>();
    //    if (!toolbar)
    //    {
    //        // This is not a toolbar event => return false.
    //        return false;
    //    }
    //    UINT message = inMessageId.messageId();
    //    if (message == WM_COMMAND)
    //    {
    //        WORD id = LOWORD(wParam);
    //        XULWin::Windows::AbstractToolbarItem * item = toolbar->nativeToolbar()->getToolbarItemByCommandId(id);
    //        if (!item)
    //        {
    //            // The command was not sent from one of the toolbar buttons. (It may have been
    //            // sent from a menu item that has the toolbar as a grandparent window.)
    //            return false;
    //        }
    //        XULWin::ToolbarButton * corrspondingToolbarButton(0);
    //        for (size_t idx = 0; idx != toolbar->getChildCount(); ++idx)
    //        {
    //            if (XULWin::ToolbarButton * button = toolbar->getChild(idx)->downcast<XULWin::ToolbarButton>())
    //            {
    //                if (button->nativeItem() == item)
    //                {
    //                    corrspondingToolbarButton = button;
    //                    break;
    //                }
    //            }
    //        }

    //        assert(corrspondingToolbarButton);
    //        if (!corrspondingToolbarButton)
    //        {
    //            return false;
    //        }

    //        LRESULT ret(cUnhandled);
    //        invokeCallbacks(MsgId(corrspondingToolbarButton->el()->parent(),
    //                              WM_COMMAND,
    //                              corrspondingToolbarButton->componentId()),
    //                        wParam,
    //                        lParam, ret);
    //        return true;
    //    }
    //    return false;
    //}


    Toolbar::Toolbar(Component * inParent, const AttributesMapping & inAttr) :
        NativeControl(inParent, inAttr)
    {
        if (NativeComponent * native = NativeControl::GetThisOrParent(inParent))
        {
            mToolbar.reset(new Windows::Toolbar(this, NativeComponent::GetModuleHandle(), native->handle(), mComponentId.value()));
            setHandle(mToolbar->handle(), false);
            registerHandle();
            subclass();
        }
    }


    Toolbar::~Toolbar()
    {
        unsubclass();
        unregisterHandle();
        mToolbar.reset();
    }


    bool Toolbar::init()
    {
        mToolbar->buildToolbar();
        mToolbar->rebuildLayout();
        ShowWindow(mToolbar->handle(), SW_SHOW);
        return Super::init();
    }


    int Toolbar::calculateWidth(SizeConstraint inSizeConstraint) const
    {
        return calculateSumChildWidths(inSizeConstraint);
    }


    int Toolbar::calculateHeight(SizeConstraint inSizeConstraint) const
    {
        int result = calculateMaxChildHeight(inSizeConstraint);
        return result;
    }


    void Toolbar::rebuildLayout()
    {
        mToolbar->rebuildLayout();
    }

} // namespace XULWin
