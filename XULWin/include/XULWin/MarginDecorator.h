#ifndef MARGINDECORATOR_H_INCLUDED
#define MARGINDECORATOR_H_INCLUDED


#include "Decorator.h"


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

} // namespace XULWin


#endif // MARGINDECORATOR_H_INCLUDED
