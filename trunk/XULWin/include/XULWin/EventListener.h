#ifndef EVENTLISTENER_H_INCLUDED
#define EVENTLISTENER_H_INCLUDED


#include "XULWin/Windows.h"
#include <boost/function.hpp>
#include <boost/noncopyable.hpp>
#include <map>
#include <vector>


namespace XULWin
{
    class Element;

    class EventListener
    {
    public:
        virtual LRESULT handleCommand(Element * inSender, WORD inNotificationCode) = 0;        

        virtual LRESULT handleMenuCommand(Element * inSender, WORD inMenuId) = 0;
        
        virtual LRESULT handleDialogCommand(Element * inSender, WORD inNotificationCode, WPARAM wParam, LPARAM lParam) = 0;

        virtual LRESULT handleMessage(Element * inSender, UINT inMessage, WPARAM wParam, LPARAM lParam) = 0;
    };


    /**
     * ScopedEventListeners enables connecting events with callbacks.
     */
    class ScopedEventListener : public EventListener,
                                boost::noncopyable
    {
    public:
        ScopedEventListener();

        virtual ~ScopedEventListener();

        typedef boost::function<LRESULT(WPARAM, LPARAM)> Action;

        // Connects element action with a callback. I.e. button push, checkbox check, etc..
        // Element is of type button, checkbox, etc...
        // It's the same as calling ScopedEventListener::connect(inEl, WM_COMMAND, inAction);
        void connect(Element * inEl, const Action & inAction);

        // Removes the connection for the element action (WM_COMMAND).
        void disconnect(Element * inEl);

        // Connects a specific windows message with a callback action.
        void connect(Element * inEl, UINT inMessage, const Action & inAction);

        // Removes the connection for a certain message.
        void disconnect(Element * inEl, UINT inMessage);

    protected:
        virtual LRESULT handleCommand(Element * inSender, WORD inNotificationCode);
        virtual LRESULT handleMenuCommand(Element * inSender, WORD inMenuId);
        virtual LRESULT handleDialogCommand(Element * inSender, WORD inNotificationCode, WPARAM wParam, LPARAM lParam) { return 1; }
        virtual LRESULT handleMessage(Element * inSender, UINT inMessage, WPARAM wParam, LPARAM lParam);

        LRESULT processMessage(Element * inSender, UINT inMessage, WPARAM wParam, LPARAM lParam);

        typedef std::pair<Element*, UINT> MsgId;
        typedef std::map<MsgId, std::vector<Action> > MessageCallbacks;
        MessageCallbacks mMessageCallbacks;
    };


} // namespace XULWin

#endif // EVENTLISTENER_H_INCLUDED
