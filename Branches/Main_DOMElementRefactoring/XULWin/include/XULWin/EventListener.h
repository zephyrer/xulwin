#ifndef EVENTLISTENER_H_INCLUDED
#define EVENTLISTENER_H_INCLUDED


#include "XULWin/Windows.h"
#include <boost/function.hpp>
#include <boost/noncopyable.hpp>
#include <map>
#include <vector>


namespace XULWin
{

    class Component;


    /**
     * Usually Windows message return 0 or 1, meaning the messages was handled, or not handled, respectively.
     * These values allow you to use symbolic names.
     */
    enum EventResult
    {
        EventResult_Handled = 0,
        EventResult_NotHandled = 1
    };


    class NativeEventListener
    {
    public:
        NativeEventListener() {}

        virtual ~NativeEventListener() {}

        const std::string & tagName() { return mType; }

        virtual LRESULT handleCommand(Component * inSender, WORD inNotificationCode, WPARAM wParam, LPARAM lParam) = 0;

        //virtual LRESULT handleMenuCommand(Component * inSender, WORD inMenuId) = 0;

        virtual LRESULT handleDialogCommand(Component * inSender, WORD inNotificationCode, WPARAM wParam, LPARAM lParam) = 0;

        virtual LRESULT handleMessage(Component * inSender, UINT inMessage, WPARAM wParam, LPARAM lParam) = 0;

    private:
        std::string mType;
    };


    /**
     * ScopedEventListener
     *
     * Enables connecting events with callbacks.
     */
    class ScopedEventListener : public NativeEventListener,
        boost::noncopyable
    {
    public:
        ScopedEventListener();

        virtual ~ScopedEventListener();

        typedef boost::function<LRESULT(WPARAM, LPARAM)> Action;


        /**
         * Connect action callbacks to your component.
         * Works with normal components (button, checkbox, ...) but also with menu items and
         * toolbar buttons.
         * This method is a shorter notation for connect(inEl, WM_COMMAND, inAction).
         */
        void connect(Component * inEl, const Action & inAction);


        /**
         * Connect a callback to a specific Windows message.
         */
        void connect(Component * inEl, UINT inMessage, const Action & inAction);


        /**
         * Connect a callback to a Windows message on a XULWin Component where the sender of the
         * message does not equal the Component that is being listened to.
         * This applies to menus and toolbar buttons.
         * Unless you want to do something special, you don't need to call this method. For normal
         * situations use the connect(Component*, const Action&) overload. It will detect and take
         * care of the special cases like menu items and toolbar buttons.
         */
        void connect(Component * inEl, UINT inMessage, int inCommandId, const Action & inAction);


        /**
         * Removes the callback connections. You don't need to call this if your ScopedEventListener
         * object is properly scoped (RAII).
         */
        void disconnect(Component * inEl);
        void disconnect(Component * inEl, UINT inMessage);
        void disconnect(Component * inEl, UINT inMessage, int inCommandId);

    protected:
        virtual LRESULT handleCommand(Component * inSender, WORD inNotificationCode, WPARAM wParam, LPARAM lParam);
        //virtual LRESULT handleMenuCommand(Component * inSender, WORD inMenuId);
        virtual LRESULT handleDialogCommand(Component * inSender, WORD inNotificationCode, WPARAM wParam, LPARAM lParam)
        {
            return 1;
        }
        virtual LRESULT handleMessage(Component * inSender, UINT inMessage, WPARAM wParam, LPARAM lParam);

        class MsgId
        {
        public:
            MsgId(Component * inComponent, UINT inMessageId, int inCommandId) :
                mComponent(inComponent),
                mMessageId(inMessageId),
                mCommandId(inCommandId)
            {
            }

            bool operator <(const MsgId & rhs) const
            {
                if (component() != rhs.component())
                {
                    return component() < rhs.component();
                }
                else if (mMessageId != rhs.mMessageId)
                {
                    return mMessageId < rhs.mMessageId;
                }
                else
                {
                    return mCommandId < rhs.mCommandId;
                }
            }

            Component * component() const
            {
                return mComponent;
            }

            UINT messageId() const
            {
                return mMessageId;
            }

            int commandId() const
            {
                return mCommandId;
            }

        private:
            Component * mComponent;
            UINT mMessageId;
            int mCommandId; // needed to identify toolbar buttons
        };
        typedef std::map<MsgId, std::vector<Action> > MessageCallbacks;

        LRESULT handleMessage(MsgId inMsgId, WPARAM wParam, LPARAM lParam);
        void invokeCallbacks(MsgId inMsgId, WPARAM wParam, LPARAM lParam, LRESULT & ret);

        //bool connectToolbarButton(Component * inEl, const Action & inAction);
        //bool connectMenuItem(Component * inEl, const Action & inAction);
        //bool handleToolbarCommand(MsgId inMessageId, WPARAM wParam, LPARAM lParam, LRESULT & ret);

        MessageCallbacks mMessageCallbacks;
    };


} // namespace XULWin

#endif // EVENTLISTENER_H_INCLUDED
