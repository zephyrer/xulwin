#ifndef EVENTLISTENER_H_INCLUDED
#define EVENTLISTENER_H_INCLUDED


#include "XULWin/Types.h"
#include "XULWin/Windows.h"
#include <boost/function.hpp>
#include <boost/noncopyable.hpp>
#include <map>
#include <vector>


namespace XULWin
{
    class Element;

    /**
     * Usually WinAPI message return 0 or 1, meaning the messages was handled, or not handled, respectively.
     * These values allow you to use symbolic names.
     */
    static const LRESULT cHandled = 0;
    static const LRESULT cUnhandled = 1;

    class EventListener
    {
    public:
        virtual LRESULT handleCommand(Element * inSender, WORD inNotificationCode, WPARAM wParam, LPARAM lParam) = 0;

        virtual LRESULT handleMenuCommand(Element * inSender, WORD inMenuId) = 0;

        virtual LRESULT handleDialogCommand(Element * inSender, WORD inNotificationCode, WPARAM wParam, LPARAM lParam) = 0;

        virtual LRESULT handleMessage(Element * inSender, UINT inMessage, WPARAM wParam, LPARAM lParam) = 0;
    };


    /**
     * ScopedEventListener
     *
     * Enables connecting events with callbacks.
     */
    class ScopedEventListener : public EventListener,
        boost::noncopyable
    {
    public:
        ScopedEventListener();

        virtual ~ScopedEventListener();

        typedef boost::function<LRESULT(WPARAM, LPARAM)> Action;


        /**
         * Connect action callbacks to your component.
         * Works with normal components (button, checkbox, ...)
         * and also with menu items and toolbar buttons.
         *
         * This method is a shorter notation for connect(inEl, WM_COMMAND, inAction).
         */
        void connect(Element * inEl, const Action & inAction);


        /**
         * Connect a callback to a specific WinAPI message.
         */
        void connect(Element * inEl, UINT inMessage, const Action & inAction);


        /**
         * Connect a callback to a WinAPI message on a XULWin Component where the sender of the
         * message does not equal the Component that is being listened to.
         * This applies to menus and toolbar buttons.
         * Unless you want to do something special, you don't need to call this method. For normal
         * situations use the connect(Element*, const Action&) overload. It will detect and take
         * care of the special cases like menu items and toolbar buttons.
         */
        void connect(Element * inEl, UINT inMessage, UInt32 inComponentId, const Action & inAction);


        /**
         * Removes the callback connections. You don't need to call this if your ScopedEventListener
         * object is properly scoped (RAII).
         */
        void disconnect(Element * inEl);
        void disconnect(Element * inEl, UINT inMessage);
        void disconnect(Element * inEl, UINT inMessage, UInt32 inComponentId);

    protected:
        bool connectMenuItem(Element * inEl, const Action & inAction);
        bool connectToolbarButton(Element * inEl, const Action & inAction);

        class MsgId
        {
        public:
            MsgId(Element * inElement, UINT inMessageId, UInt32 inComponentId) :
                mElement(inElement),
                mMessageId(inMessageId),
                mComponentId(inComponentId)
            {
            }

            bool operator <(const MsgId & rhs) const
            {
                if (mElement != rhs.mElement)
                {
                    return mElement < rhs.mElement;
                }
                else if (mMessageId != rhs.mMessageId)
                {
                    return mMessageId < rhs.mMessageId;
                }
                else
                {
                    return mComponentId < rhs.mComponentId;
                }
            }

            Element * element() const
            {
                return mElement;
            }

            UINT messageId() const
            {
                return mMessageId;
            }

            UInt32 componentId() const
            {
                return mComponentId;
            }

        private:
            Element * mElement;
            UINT mMessageId;
            UInt32 mComponentId; // needed to identify toolbar buttons
        };

        virtual LRESULT handleCommand(Element * inSender, WORD inNotificationCode, WPARAM wParam, LPARAM lParam);
        virtual LRESULT handleMenuCommand(Element * inSender, WORD inMenuId);
        virtual LRESULT handleDialogCommand(Element * inSender, WORD inNotificationCode, WPARAM wParam, LPARAM lParam);
        virtual LRESULT handleMessage(Element * inSender, UINT inMessage, WPARAM wParam, LPARAM lParam);

        LRESULT handleMessage(MsgId inMsgId, WPARAM wParam, LPARAM lParam);        
        bool handleToolbarCommand(MsgId inMessageId, WPARAM wParam, LPARAM lParam, LRESULT & ret);
        void invokeCallbacks(MsgId inMsgId, WPARAM wParam, LPARAM lParam, LRESULT & ret);

        typedef std::map<MsgId, std::vector<Action> > MessageCallbacks;
        MessageCallbacks mMessageCallbacks;
    };


} // namespace XULWin

#endif // EVENTLISTENER_H_INCLUDED
