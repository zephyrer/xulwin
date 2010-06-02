#ifndef DECORATOR_H_INCLUDED
#define DECORATOR_H_INCLUDED


#include "XULWin/Component.h"
#include "XULWin/ScrollBar.h"
#include "XULWin/EventListener.h"


namespace XULWin
{

    class Decorator : public Component
    {
    public:
        typedef Component Super;

        static ComponentPtr Create(Component * inDecoratedComponent)
        {
            ComponentPtr result(new Decorator(inDecoratedComponent));
            return result;
        }

        static ComponentPtr Create(ComponentPtr inDecoratedComponent)
        {
            ComponentPtr result(new Decorator(inDecoratedComponent));
            return result;
        }

        // This is the usual constructor, called by factory methods.
        Decorator(Component * inDecoratedComponent);

        // This allows you to create a component first and wrap in a decorator later.
        Decorator(ComponentPtr inDecoratedComponent);

        virtual ~Decorator();

        virtual bool init();

        virtual int getIndex() const;

        virtual size_t getChildCount() const;

        virtual const Component * getChild(size_t inIndex) const;

        virtual Component * getChild(size_t inIndex);

        virtual HWND getFirstParentHandle();

        virtual void invalidateRect() const;

        // WidthController methods
        virtual int getWidth() const;

        virtual void setWidth(int inWidth);

        // HeightController methods
        virtual int getHeight() const;

        virtual void setHeight(int inHeight);

        // ScreenXController methods
        virtual int getScreenX() const;

        virtual void setScreenX(int inX);

        // ScreenYController methods
        virtual int getScreenY() const;

        virtual void setScreenY(int inY);

        // SVG_StrokeController methods
        virtual void setSVGStroke(const RGBColor & inColor);

        virtual const RGBColor & getSVGStroke() const;

        // SVG_StrokeWidthController methods
        virtual void setSVGStrokeWidth(int inStrokeWidth);

        virtual int getSVGStrokeWidth() const;

        // SVG_FillController methods
        virtual void setSVGFill(const RGBColor & inColor);

        virtual const RGBColor & getSVGFill() const;

        // FlexController methods
        virtual int getFlex() const;

        virtual void setFlex(int inFlex);

        // HiddenController methods
        virtual bool isHidden() const;

        virtual void setHidden(bool inHidden);

        // OrientController methods
        virtual Orient getOrient() const;

        virtual void setOrient(Orient inOrient);

        // AlignController methods
        virtual Align getAlign() const;

        virtual void setAlign(Align inAlign);

        // CSSWidthController methods
        virtual int getCSSWidth() const;

        virtual void setCSSWidth(int inWidth);

        // CSSHeightController methods
        virtual int getCSSHeight() const;

        virtual void setCSSHeight(int inHeight);

        // CSSBackgroundColor methods
        virtual void setCSSBackgroundColor(const RGBColor & inColor);

        virtual RGBColor getCSSBackgroundColor() const;

        // CSSMarginController methods
        virtual void getCSSMargin(int & outTop, int & outLeft, int & outRight, int & outBottom) const;

        virtual void setCSSMargin(int inTop, int inLeft, int inRight, int inBottom);

        // CSS_SVG_FillController methods
        virtual void setCSSFill(const RGBColor & inColor);

        virtual const RGBColor & getCSSFill() const;

        // CSS_SVG_StrokeController methods
        virtual void setCSSStroke(const RGBColor & inColor);

        virtual const RGBColor & getCSSStroke() const;

        // CSSXController methods
        virtual int getCSSX() const;

        virtual void setCSSX(int inX);

        // CSSYController methods
        virtual int getCSSY() const;

        virtual void setCSSY(int inY);

        virtual UInt32 componentId() const;

        virtual int getWidth(SizeConstraint inSizeConstraint) const;

        virtual int getHeight(SizeConstraint inSizeConstraint) const;

        virtual int calculateWidth(SizeConstraint inSizeConstraint) const;

        virtual int calculateHeight(SizeConstraint inSizeConstraint) const;

        // Tendency to expand, used for separators, scrollbars, etc..
        virtual bool expansive() const;

        virtual void move(int x, int y, int w, int h);

        virtual void move(const Rect & inRect);

        virtual Rect clientRect() const;

        virtual void setOwningElement(Element * inElement);

        virtual Element * el() const;

        virtual Component * parent() const;

        virtual void rebuildLayout();

        virtual void rebuildChildLayouts();

        virtual LRESULT handleMessage(UINT inMessage, WPARAM wParam, LPARAM lParam);

        virtual bool initAttributeControllers();

        virtual bool initStyleControllers();

        ComponentPtr decoratedElement() const;

        void setDecoratedComponent(ComponentPtr inElement);

        // Takes ownership, and destroys any previously set element
        void setDecoratedComponent(Component * inElement);

        virtual bool getAttribute(const std::string & inName, std::string & outValue);

        virtual bool getStyle(const std::string & inName, std::string & outValue);

        virtual bool setAttribute(const std::string & inName, const std::string & inValue);

        virtual bool setStyle(const std::string & inName, const std::string & inValue);

        virtual void onChildAdded(Component * inChild);

        virtual void onChildRemoved(Component * inChild);

        virtual void onContentChanged();

    protected:
        ComponentPtr mDecoratedComponent;
    };


    class WrapDecorator : public Decorator
    {
    public:
        typedef Decorator Super;

        WrapDecorator(Component * inParent, Component * inDecoratedComponent);

        // takes ownership
        void addChild(ElementPtr inChild);

    protected:
        Component * mParent;
        std::vector<ElementPtr> mDecoratorChildren;
    };


    class ScrollDecorator : public Decorator,
                            public ScrollBar::EventListener
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

        virtual bool curposChanged(ScrollBar * inSender, int inOldPos, int inNewPos);

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


#endif // DECORATOR_H_INCLUDED
