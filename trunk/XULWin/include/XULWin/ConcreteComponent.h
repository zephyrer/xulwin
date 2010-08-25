#ifndef CONCRETECOMPONENT_H_INCLUDED
#define CONCRETECOMPONENT_H_INCLUDED


#include "XULWin/Component.h"
#include "XULWin/Fallible.h"
#include <map>
#include <string>


namespace XULWin
{

    class Window;

    /**
     * ConcreteComponent is the base class for non-decorator components.
     */
    class ConcreteComponent : public Component
    {
    public:
        ConcreteComponent(Component * inParent);

        virtual ~ConcreteComponent() = 0;

        virtual bool init();

        virtual Window * findParentWindow();

        virtual const Window * findParentWindow() const;

        virtual int getIndex() const;

        virtual size_t getChildCount() const;

        virtual const Component * getChild(size_t inIndex) const;

        virtual Component * getChild(size_t inIndex);

        virtual void invalidateRect() const;

        // WidthController methods
        // Returns value from last call to setWidth. If setWidth has not yet
        // been called, then this method returns the value as defined in XUL
        // document. If the value in the XUL document has not been defined,
        // then the optimal size of the element is returned.
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

        // CSSBackgroundColor methods
        virtual void setCSSBackgroundColor(const RGBColor & inColor);

        virtual RGBColor getCSSBackgroundColor() const;

        // CSSHeightController methods
        virtual int getCSSHeight() const;

        virtual void setCSSHeight(int inHeight);

        // CSSMarginController methods
        virtual void getCSSMargin(int & outTop, int & outLeft, int & outRight, int & outBottom) const;

        virtual void setCSSMargin(int inTop, int inLeft, int inRight, int inBottom);

        // CSSFillController methods
        virtual void setCSSFill(const RGBColor & inColor);

        virtual const RGBColor & getCSSFill() const;

        // CSSStrokeController methods
        virtual void setCSSStroke(const RGBColor & inColor);

        virtual const RGBColor & getCSSStroke() const;

        // CSSXController methods
        virtual int getCSSX() const;

        virtual void setCSSX(int inX);

        // CSSYController methods
        virtual int getCSSY() const;

        virtual void setCSSY(int inY);

        // Downcast that also resolves decorators.
        // Use this instead of manual cast, because
        // you may get a decorator instead of the
        // actual element.
        template<class Type>
        Type * downcast()
        {
            if (Type * obj = dynamic_cast<Type *>(this))
            {
                return obj;
            }
            else if (Decorator * obj = dynamic_cast<Decorator *>(this))
            {
                return obj->decoratedElement()->downcast<Type>();
            }
            return 0;
        }


        template<class ConstType>
        const ConstType * downcast() const
        {
            if (const ConstType * obj = dynamic_cast<const ConstType *>(this))
            {
                return obj;
            }
            else if (const Decorator * obj = dynamic_cast<const Decorator *>(this))
            {
                return obj->decoratedElement()->downcast<ConstType>();
            }
            return 0;
        }

        UInt32 componentId() const
        {
            return mComponentId.value();
        }

        virtual int getWidth(SizeConstraint inSizeConstraint) const;

        virtual int getHeight(SizeConstraint inSizeConstraint) const;

        virtual int calculateWidth(SizeConstraint inSizeConstraint) const = 0;

        virtual int calculateHeight(SizeConstraint inSizeConstraint) const = 0;

        // Tendency to expand, used for separators, scrollbars, etc..
        bool expansive() const;

        virtual void move(int x, int y, int w, int h) = 0;

        virtual void move(const Rect & inRect);

        virtual Rect clientRect() const = 0;

        virtual void setOwningElement(Element * inElement);

        virtual Element * el() const;

        Component * parent() const;

        virtual void rebuildLayout() = 0;

        void rebuildChildLayouts();

        virtual LRESULT handleMessage(UINT inMessage, WPARAM wParam, LPARAM lParam) = 0;

        virtual bool getAttribute(const std::string & inName, std::string & outValue);

        virtual bool getStyle(const std::string & inName, std::string & outValue);

        virtual bool setStyle(const std::string & inName, const std::string & inValue);

        virtual bool setAttribute(const std::string & inName, const std::string & inValue);

        virtual bool initAttributeControllers();

        virtual bool initStyleControllers();

        template<class T>
        void setAttributeController(T * inAttributeController)
        {
            AttributeControllers::iterator it = mAttributeControllers.find(T::AttributeName());
            assert(it == mAttributeControllers.end());
            if (it == mAttributeControllers.end())
            {
                mAttributeControllers.insert(std::make_pair(T::AttributeName(), inAttributeController));
            }
        }

        template<class T>
        void setStyleController(T * inStyleController)
        {
            StyleControllers::iterator it = mStyleControllers.find(T::PropertyName());
            assert(it == mStyleControllers.end());
            if (it == mStyleControllers.end())
            {
                mStyleControllers.insert(std::make_pair(T::PropertyName(), inStyleController));
            }
        }

        int calculateMaxChildWidth(SizeConstraint inSizeConstraint) const;

        int calculateMaxChildHeight(SizeConstraint inSizeConstraint) const;

        int calculateSumChildWidths(SizeConstraint inSizeConstraint) const;

        int calculateSumChildHeights(SizeConstraint inSizeConstraint) const;

    protected:
        Component * mParent;
        Element * mElement;
        ComponentId mComponentId;
        bool mExpansive;
        int mFlex;

        Fallible<int> mScreenX;
        Fallible<int> mScreenY;
        Fallible<int> mCSSX;
        Fallible<int> mCSSY;
        Fallible<int> mWidth;
        Fallible<int> mHeight;
        Fallible<int> mCSSWidth;
        Fallible<int> mCSSHeight;
        Fallible<int> mStrokeWidth;
        Fallible<RGBColor> mCSSFill;
        Fallible<RGBColor> mCSSStroke;
        Fallible<RGBColor> mCSSBackgroundColor;
        Fallible<Orient> mOrient;
        Fallible<Align> mAlign;
        bool mIsInitialized;

        // We need to remember the hidden state ourselves because we can't
        // rely on WinAPI IsWindowVisible call, because it will return false
        // for child windows of which the parent is not visible.
        // This is a problem because calculation of minimum size depends on
        // visibility of child items. Hidden elements return 0 when asked for
        // their min width or height. This problem becomes apparent when
        // calling Window's showModal method which calculates its minimum
        // height *before* the window becomes visible. This would result in a
        // 'minified' window state.
        bool mHidden;

        typedef std::map<std::string, AttributeController *> AttributeControllers;
        AttributeControllers mAttributeControllers;

        typedef std::map<std::string, StyleController *> StyleControllers;
        StyleControllers mStyleControllers;
    };

} // namespace XULWin


#endif // CONCRETECOMPONENT_H_INCLUDED
