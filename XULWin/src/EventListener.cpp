#include "XULWin/EventListener.h"
#include "XULWin/Component.h"
#include "XULWin/Decorator.h"
#include "XULWin/Elements.h"
#include "XULWin/ErrorReporter.h"
#include "XULWin/MenuItemElement.h"
#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>


namespace XULWin
{


    ScopedEventListener::ScopedEventListener()
    {
    }


    ScopedEventListener::~ScopedEventListener()
    {
        while (!mMessageCallbacks.empty())
        {
            const MsgId & id = mMessageCallbacks.begin()->first;
            id.element()->removeEventListener(this);
            mMessageCallbacks.erase(mMessageCallbacks.begin());
        }
    }


    /**
     * We can't connect to a ToolbarButtonElement because in the WinAPI the
     * toolbar buttons aren't really windows and they don't send WM_COMMAND
     * events. It's the toolbar window that sends the messages, and the buttons
     * are identified with their ids.
     * So to make it work in XULWin we must connect ToolbarElements with their
     * event handlers and not ToolbarButtons. This method will catch any
     * attempts to connect toolbar buttons and connect the toolbar instead.
     * The event handling code must be redirected as well of course. And this is
     * taken care of in the handleToolbarCommand() call below.
     */
    bool ScopedEventListener::connectToolbarButton(Element * inEl, const Action & inAction)
    {
        if (!inEl->parent())
        {
            return false;
        }

        if (XULWin::ToolbarElement * toolbar = inEl->parent()->downcast<XULWin::ToolbarElement>())
        {
            connect(toolbar, WM_COMMAND, inEl->component()->componentId(), inAction);
            return true;
        }
        return false;
    }


    bool ScopedEventListener::connectMenuItem(Element * inEl, const Action & inAction)
    {
        if (0 == inEl->downcast<XULWin::MenuItemElement>())
        {
            // This is not a menu item.
            return false;
        }

        XULWin::NativeComponent * nativeParent = NativeControl::GetThisOrParent(inEl->component());
        if (!nativeParent)
        {
            ReportError("Received an event from a MenuItem element that has no native parent.");
            return false;
        }

        connect(nativeParent->el(), WM_COMMAND, inEl->component()->componentId(), inAction);
        return true;
    }


    void ScopedEventListener::connect(Element * inEl, const Action & inAction)
    {
        if (!connectToolbarButton(inEl, inAction) &&
                !connectMenuItem(inEl, inAction))
        {
            connect(inEl, WM_COMMAND, inAction);
        }
    }


    void ScopedEventListener::connect(Element * inEl, UINT inMessage, const Action & inAction)
    {
        connect(inEl, inMessage, inEl->component()->componentId(), inAction);
    }


    void ScopedEventListener::connect(Element * inEl, UINT inMessage, UInt32 inComponentId, const Action & inAction)
    {
        if (inEl)
        {
            inEl->addEventListener(this);
            mMessageCallbacks[MsgId(inEl, inMessage, inComponentId)].push_back(inAction);
        }
    }


    void ScopedEventListener::disconnect(Element * inEl)
    {
        disconnect(inEl, WM_COMMAND, inEl->component()->componentId());
    }


    void ScopedEventListener::disconnect(Element * inEl, UINT inMessage)
    {
        disconnect(inEl, inMessage, inEl->component()->componentId());
    }


    void ScopedEventListener::disconnect(Element * inEl, UINT inMessage, UInt32 inComponentId)
    {
        if (inEl)
        {
            MessageCallbacks::iterator it = mMessageCallbacks.find(MsgId(inEl, inMessage, inComponentId));
            if (it != mMessageCallbacks.end())
            {
                it->first.element()->removeEventListener(this);
                mMessageCallbacks.erase(it);
            }
        }
    }


    /**
     * In the WinAPI toolbar commands are sent from the toolbar window to its parent window.
     * We need to find out which toolbar button sent the message, and then
     * find the corresponding ToolbarButton element object.
     * Once we found this object we can look for any associated callbacks.
     */
    bool ScopedEventListener::handleToolbarCommand(MsgId inMessageId,
                                                   WPARAM wParam,
                                                   LPARAM lParam,
                                                   LRESULT & ret)
    {
        XULWin::Toolbar * toolbar = inMessageId.element()->component()->downcast<XULWin::Toolbar>();
        if (!toolbar)
        {
            // This is not a toolbar event => return false.
            return false;
        }
        UINT message = inMessageId.messageId();
        if (message == WM_COMMAND)
        {
            WORD id = LOWORD(wParam);
            XULWin::Windows::AbstractToolbarItem * item = toolbar->nativeToolbar()->getToolbarItemByCommandId(id);
            if (!item)
            {
                // The command was not sent from one of the toolbar buttons. (It may have been
                // sent from a menu item that has the toolbar as a grandparent window.)
                return false;
            }
            XULWin::ToolbarButton * corrspondingToolbarButton(0);
            for (size_t idx = 0; idx != toolbar->getChildCount(); ++idx)
            {
                if (XULWin::ToolbarButton * button = toolbar->getChild(idx)->downcast<XULWin::ToolbarButton>())
                {
                    if (button->nativeItem() == item)
                    {
                        corrspondingToolbarButton = button;
                        break;
                    }
                }
            }

            assert(corrspondingToolbarButton);
            if (!corrspondingToolbarButton)
            {
                return false;
            }

            LRESULT ret(cUnhandled);
            invokeCallbacks(MsgId(corrspondingToolbarButton->el()->parent(),
                                  WM_COMMAND,
                                  corrspondingToolbarButton->componentId()),
                            wParam,
                            lParam, ret);
            return true;
        }
        return false;
    }


    void ScopedEventListener::invokeCallbacks(MsgId inMsgId, WPARAM wParam, LPARAM lParam, LRESULT & ret)
    {
        MessageCallbacks::iterator it = mMessageCallbacks.find(inMsgId);
        if (it == mMessageCallbacks.end())
        {
            ret = cUnhandled;
            return;
        }

        ret = cHandled;
        std::vector<Action> callbacks = it->second;
        for (size_t idx = 0; idx != callbacks.size(); ++idx)
        {
            if (callbacks[idx])
            {
                if (cUnhandled == callbacks[idx](wParam, lParam))
                {
                    // If there is one callback that says it didn't handle the message, then
                    // we consider the entire message unhandled. The reasoning behind this is
                    // that in case of doubt, we choose to consider the message unhandled. This
                    // is important because in our subclasses windows we only call the original
                    // proc (through CallWindowProc) in case of unhandled messages.
                    ret = cUnhandled;
                }
            }
        }
    }


    LRESULT ScopedEventListener::handleMessage(MsgId inMsgId, WPARAM wParam, LPARAM lParam)
    {
        LRESULT ret = cUnhandled;
        if (!handleToolbarCommand(inMsgId, wParam, lParam, ret))
        {
            invokeCallbacks(inMsgId, wParam, lParam, ret);
        }
        return ret;
    }


    LRESULT ScopedEventListener::handleCommand(Element * inSender, WORD inNotificationCode, WPARAM wParam, LPARAM lParam)
    {
        return handleMessage(MsgId(inSender,
                                   WM_COMMAND,
                                   LOWORD(wParam)),
                             wParam,
                             lParam);
    }


    LRESULT ScopedEventListener::handleMenuCommand(Element * inSender, WORD inMenuId)
    {
        return handleMessage(MsgId(inSender,
                                   WM_COMMAND,
                                   inMenuId),
                             0,
                             0);
    }


    LRESULT ScopedEventListener::handleMessage(Element * inSender, UINT inMessage, WPARAM wParam, LPARAM lParam)
    {
        return handleMessage(MsgId(inSender,
                                   inMessage,
                                   inSender->component()->componentId()), // LOWORD(wParam)),
                             wParam,
                             lParam);
    }


} // namespace XULWin
