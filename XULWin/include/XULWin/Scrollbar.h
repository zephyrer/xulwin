#ifndef SCROLLBAR_H_INCLUDED
#define SCROLLBAR_H_INCLUDED


#include "NativeControl.h"


namespace XULWin
{

    class Scrollbar : public NativeControl,
                      public virtual ScrollbarCurrentPositionController,
                      public virtual ScrollbarMaxPositionController,
                      public virtual ScrollbarIncrementController,
                      public virtual ScrollbarPageIncrementController
    {
    public:
        typedef NativeControl Super;

        Scrollbar(Component * inParent, const AttributesMapping & inAttributesMapping);

        virtual int getCurrentPosition() const;

        virtual void setCurrentPosition(int inCurrentPosition);

        virtual int getMaxPosition() const;

        virtual void setMaxPosition(int inMaxPosition);

        virtual int getIncrement() const;

        virtual void setIncrement(int inIncrement);

        virtual int getPageIncrement() const;

        virtual void setPageIncrement(int inPageIncrement);

        class EventListener
        {
        public:
            virtual bool curposChanged(Scrollbar * inSender, int inOldPos, int inNewPos) = 0;
        };

        EventListener * eventHandler()
        {
            return mEventListener;
        }

        void setEventListener(EventListener * inEventListener)
        {
            mEventListener = inEventListener;
        }

        virtual int calculateWidth(SizeConstraint inSizeConstraint) const;

        virtual int calculateHeight(SizeConstraint inSizeConstraint) const;

        bool initAttributeControllers();

        virtual LRESULT handleMessage(UINT inMessage, WPARAM wParam, LPARAM lParam);

    private:
        static DWORD GetFlags(const AttributesMapping & inAttributesMapping);

        EventListener * mEventListener;
        int mIncrement;
    };

} // namespace XULWin


#endif // SCROLLBAR_H_INCLUDED
