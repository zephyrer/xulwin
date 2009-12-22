#include "XULWin/EventListener.h"
#include "XULWin/Decorator.h"
#include "XULWin/Component.h"
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
            connect(toolbar, WM_COMMAND, inEl->component()->commandId(), inAction);
            return true;
        }
        return false;
    }


    void ScopedEventListener::connect(Element * inEl, const Action & inAction)
    {
        if (!connectToolbarButton(inEl, inAction))
        {
            connect(inEl, WM_COMMAND, inAction);
        }
    }


    void ScopedEventListener::connect(Element * inEl, UINT inMessage, const Action & inAction)
    {
        connect(inEl, inMessage, inEl->component()->commandId(), inAction);
    }


    void ScopedEventListener::connect(Element * inEl, UINT inMessage, int inCommandId, const Action & inAction)
    {
        if (inEl)
        {
            inEl->addEventListener(this);
            mMessageCallbacks[MsgId(inEl, inMessage, inCommandId)].push_back(inAction);
        }
    }


    void ScopedEventListener::disconnect(Element * inEl)
    {
        disconnect(inEl, WM_COMMAND, inEl->component()->commandId());
    }

    
    void ScopedEventListener::disconnect(Element * inEl, UINT inMessage)
    {
        disconnect(inEl, inMessage, inEl->component()->commandId());
    }

    
    void ScopedEventListener::disconnect(Element * inEl, UINT inMessage, int inCommandId)
    {
        if (inEl)
        {
            MessageCallbacks::iterator it = mMessageCallbacks.find(MsgId(inEl, inMessage, inCommandId));
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
    bool ScopedEventListener::handleToolbarCommand(const MessageCallbacks::iterator & inIterator,
                                                   WPARAM wParam,
                                                   LPARAM lParam,
                                                   LRESULT & ret)
    {
        Element * element = inIterator->first.element();
        XULWin::Toolbar * toolbar = element->component()->downcast<XULWin::Toolbar>();
        if (!toolbar)
        {
            // This is not a toolbar event => return false.
            return false;
        }
        UINT message = inIterator->first.messageId();
        if (message == WM_COMMAND)
        {            
	      	WORD id = LOWORD(wParam);
            XULWin::Windows::AbstractToolbarItem * item = toolbar->nativeToolbar()->getToolbarItemByCommandId(id);
            assert(item);
            if (!item)
            {
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


            MessageCallbacks::iterator it = mMessageCallbacks.find(
                MsgId(corrspondingToolbarButton->el()->parent(),
                      WM_COMMAND,
                      corrspondingToolbarButton->commandId()));

            if (it != mMessageCallbacks.end())
            {
                std::vector<Action> callbacks = it->second;
                for (size_t idx = 0; idx != callbacks.size(); ++idx)
                {
                    if (callbacks[idx])
                    {
                        ret = callbacks[idx](wParam, lParam);
                    }
                }
            }
            return true;
        }
        return false;
    }


    LRESULT ScopedEventListener::processMessage(Element * inSender, UINT inMessage, WPARAM wParam, LPARAM lParam)
    {
        LRESULT ret(0);
        int commandId = inSender->component()->commandId();
        if (inMessage == WM_COMMAND)
        {
            commandId = LOWORD(wParam);
        }

        MessageCallbacks::iterator it = mMessageCallbacks.find(MsgId(inSender, inMessage, commandId));
        if (it != mMessageCallbacks.end())
        {
            if (!handleToolbarCommand(it, wParam, lParam, ret))
            {
                std::vector<Action> callbacks = it->second;
                for (size_t idx = 0; idx != callbacks.size(); ++idx)
                {
                    if (callbacks[idx])
                    {
                        ret = callbacks[idx](wParam, lParam);
                    }
                }
            }
        }
        return ret;
    }

        
    LRESULT ScopedEventListener::handleCommand(Element * inSender, WORD inNotificationCode, WPARAM wParam, LPARAM lParam)
    {
        return processMessage(inSender, WM_COMMAND, wParam, lParam);
    }


    LRESULT ScopedEventListener::handleMenuCommand(Element * inSender, WORD inMenuId)
    {
        return processMessage(inSender, WM_COMMAND, 0, 0);
    }
    

    LRESULT ScopedEventListener::handleMessage(Element * inSender, UINT inMessage, WPARAM wParam, LPARAM lParam)
    {
        return processMessage(inSender, inMessage, wParam, lParam);
    }


} // namespace XULWin
