#include "XULWin/EventListener.h"
#include "XULWin/Component.h"
#include "XULWin/Components.h"
#include "XULWin/Decorator.h"
#include "XULWin/Elements.h"
#include "XULWin/ErrorReporter.h"
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


    bool ScopedEventListener::connectMenuItem(Element * inEl, const Action & inAction)
    {
        if (0 == inEl->downcast<XULWin::XMLMenuItem>())
        {
            // This is not a menu item.
            return false;
        }

        XULWin::NativeComponent * nativeParent = NativeControl::FindNativeParent(inEl->component());
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
        if (!connectMenuItem(inEl, inAction))
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
        LRESULT ret;
        invokeCallbacks(inMsgId, wParam, lParam, ret);
        return cUnhandled;
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
