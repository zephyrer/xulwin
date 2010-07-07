#ifndef DECORATORS_H_INCLUDED
#define DECORATORS_H_INCLUDED


#include "XULWin/Decorator.h"


namespace XULWin
{

    class MarginDecorator : public Decorator
    {
    public:
        typedef Decorator Super;

        // Takes ownership.
        MarginDecorator(Component * inDecoratedComponent);

        // This constructor is needed for insertion of new objects in the Decorator chain.
        MarginDecorator(ComponentPtr inDecoratedComponent);

        virtual ~MarginDecorator();

        void setMargin(int top, int left, int right, int bottom);

        void getMargin(int & outTop, int & outLeft, int & outRight, int & outBottom) const;

        int marginTop() const;

        int marginLeft() const;

        int marginRight() const;

        int marginBottom() const;

        virtual int getWidth() const;

        virtual void setWidth(int inWidth);

        virtual int getHeight() const;

        virtual void setHeight(int inHeight);

        virtual int getWidth(SizeConstraint inSizeConstraint) const;

        virtual int getHeight(SizeConstraint inSizeConstraint) const;

        virtual Rect clientRect() const;

        virtual void move(const Rect & inRect);

        virtual void move(int x, int y, int w, int h);

        virtual int calculateWidth(SizeConstraint inSizeConstraint) const;

        virtual int calculateHeight(SizeConstraint inSizeConstraint) const;

    private:
        int mTop, mLeft, mRight, mBottom;
        Rect mOuterRect;
        Rect mInnerRect;
    };


    class ScrollDecorator : public Decorator,
                            public Scrollbar::EventListener
    {
    public:
        typedef Decorator Super;

        // Takes ownership.
        ScrollDecorator(Component * inParent,
                        Component * inDecoratedComponent,
                        CSSOverflow inOverflowX,
                        CSSOverflow inOverflowY);

        virtual bool init();

        virtual int calculateWidth(SizeConstraint inSizeConstraint) const;

        virtual int calculateHeight(SizeConstraint inSizeConstraint) const;

        virtual int getWidth(SizeConstraint inSizeConstraint) const;

        virtual int getHeight(SizeConstraint inSizeConstraint) const;

        virtual void move(int x, int y, int w, int h);

        virtual void rebuildLayout();

        virtual bool curposChanged(Scrollbar * inSender, int inOldPos, int inNewPos);

        LRESULT handleMouseWheel(WPARAM wParam, LPARAM lParam);

    private:
        void updateWindowScroll();
        void updateHorizontalScrollInfo();
        void updateVerticalScrollInfo();

        CSSOverflow mOverflowX;
        CSSOverflow mOverflowY;
        int mOldHorScrollPos;
        int mOldVerScrollPos;
        ElementPtr mVerticalScrollbar;
        ElementPtr mHorizontalScrollbar;
        ScopedEventListener mEvents;
    };

} // namespace XULWin


#endif // DECORATORS_H_INCLUDED
