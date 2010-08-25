#ifndef COMPONENT_H_INCLUDED
#define COMPONENT_H_INCLUDED


#include "XULWin/AttributeController.h"
#include "XULWin/AttributesMapping.h"
#include "XULWin/Enums.h"
#include "XULWin/ForwardDeclarations.h"
#include "XULWin/Rect.h"
#include "XULWin/StyleController.h"
#include "XULWin/UniqueId.h"
#include "XULWin/Windows.h"
#include <boost/noncopyable.hpp>


namespace XULWin
{

    DECLARE_UNIQUE_ID(ComponentId, 101)


    /**
     * Listener for XUL notifications
     */
    class NotificationListener
    {
    public:
        // This notification is sent just after the child has been added.
        virtual void onChildAdded(Component * inChild) {}

        // This notification is sent after the child has been removed and
        // just before the child is destroyed.
        virtual void onChildRemoved(Component * inChild) {}

        virtual void onContentChanged() {}
    };


    class Window;


    /**
     * Component
     *
     * The Component class is a C++ interface for a XUL element.
     * It provides methods for getting and setting attributes.
     *
     * Each xul attribute that has an effect on what the native UI looks like
     * requires a matching AttributeController class (a class that inherits
     * AttributeController) that implements the conversion from/to the typed
     * attribute value to a string value (XML attributes are strings).
     */
    class Component : public NotificationListener,
        public virtual AlignController,
        public virtual CSSBackgroundColorController,
        public virtual CSSHeightController,
        public virtual CSSMarginController,
        public virtual CSSWidthController,
        public virtual CSSXController,
        public virtual CSSYController,
        public virtual CSSFillController,
        public virtual CSSStrokeController,
        public virtual FlexController,
        public virtual HeightController,
        public virtual HiddenController,
        public virtual OrientController,
        public virtual ScreenXController,
        public virtual ScreenYController,
        public virtual WidthController,
        private boost::noncopyable
    {
    public:
        virtual ~Component() {}

        virtual bool init() = 0;

        virtual Window * findParentWindow() = 0;

        virtual const Window * findParentWindow() const = 0;

        // Returns this element's index in its parent's children collection.
        virtual int getIndex() const = 0;

        virtual size_t getChildCount() const = 0;

        virtual const Component * getChild(size_t inIndex) const = 0;

        virtual Component * getChild(size_t inIndex) = 0;

        virtual void invalidateRect() const = 0;

        // WidthController methods
        // Returns value from last call to setWidth. If setWidth has not yet
        // been called, then this method returns the value as defined in XUL
        // document. If the value in the XUL document has not been defined,
        // then the optimal size of the element is returned.
        virtual int getWidth() const = 0;

        virtual void setWidth(int inWidth) = 0;

        // HeightController methods
        virtual int getHeight() const = 0;

        virtual void setHeight(int inHeight) = 0;

        // ScreenXController methods
        virtual int getScreenX() const = 0;

        virtual void setScreenX(int inX) = 0;

        // ScreenYController methods
        virtual int getScreenY() const = 0;

        virtual void setScreenY(int inY) = 0;

        // FlexController methods
        virtual int getFlex() const = 0;

        virtual void setFlex(int inFlex) = 0;

        // HiddenController methods
        virtual bool isHidden() const = 0;

        virtual void setHidden(bool inHidden) = 0;

        // OrientController methods
        virtual Orient getOrient() const = 0;

        virtual void setOrient(Orient inOrient) = 0;

        // AlignController methods
        virtual Align getAlign() const = 0;

        virtual void setAlign(Align inAlign) = 0;

        // CSSBackgroundColor methods
        virtual void setCSSBackgroundColor(const RGBColor & inColor) = 0;

        virtual RGBColor getCSSBackgroundColor() const = 0;

        // CSSWidthController methods
        virtual int getCSSWidth() const = 0;

        virtual void setCSSWidth(int inWidth) = 0;

        // CSSHeightController methods
        virtual int getCSSHeight() const = 0;

        virtual void setCSSHeight(int inHeight) = 0;

        // CSSMarginController methods
        virtual void getCSSMargin(int & outTop, int & outLeft, int & outRight, int & outBottom) const = 0;

        virtual void setCSSMargin(int inTop, int inLeft, int inRight, int inBottom) = 0;

        // CSS_SVG_FillController methods
        virtual void setCSSFill(const RGBColor & inColor) = 0;

        virtual const RGBColor & getCSSFill() const = 0;

        // CSS_SVG_StrokeController methods
        virtual void setCSSStroke(const RGBColor & inColor) = 0;

        virtual const RGBColor & getCSSStroke() const = 0;

        // CSSXController methods
        virtual int getCSSX() const = 0;

        virtual void setCSSX(int inX) = 0;

        // CSSYController methods
        virtual int getCSSY() const = 0;

        virtual void setCSSY(int inY) = 0;

        // Downcast that also resolves decorators.
        // Don't use dynamic_cast, because that would almost always fail since
        // Components are usually wrapped in Decorator objects.
        template<class Type>
        Type * downcast()
        {
            return const_cast<Type *>(static_cast<const Component *>(this)->downcast<Type>());
        }

        // Downcast that also resolves decorators.
        // Don't use dynamic_cast, because that would almost always fail since
        // Components are usually wrapped in Decorator objects.
        template<class Type>
        const Type * downcast() const
        {
            if (const Type * obj = dynamic_cast<const Type *>(this))
            {
                return obj;
            }
            //
            // NOTE: if you get the following compiler error:
            //  "error C2680: 'const XULWin::Decorator *' : invalid target type for dynamic_cast"
            // then that means that you must include "XULWin/Decorator.h" in your code.
            //
            else if (const Decorator * obj = dynamic_cast<const Decorator *>(this))
            {
                return obj->decoratedElement()->downcast<Type>();
            }
            return 0;
        }


        template<class Type>
        Type * findParentOfType()
        {
            if (Type * obj = dynamic_cast<Type *>(this))
            {
                return obj;
            }
            else if (const Decorator * obj = dynamic_cast<const Decorator *>(this))
            {
                return obj->decoratedElement()->findParentOfType<Type>();
            }
            else if (el() && el()->parent() && el()->parent()->component())
            {
                return el()->parent()->component()->findParentOfType<Type>();
            }
            return 0;
        }


        // Searches for a child of given type.
        // Returns the first one found.
        // Only searches one level deep.
        template<class Type>
        Type * findChildOfType()
        {
            return const_cast<Type *>(static_cast<const Component *>(this)->findChildOfType<Type>());
        }


        // Searches for a child of given type.
        // Returns the first one found.
        // Only searches one level deep.
        template<class Type>
        const Type * findChildOfType() const
        {
            if (!el())
            {
                return 0;
            }

            for (size_t idx = 0; idx != getChildCount(); ++idx)
            {
                const Component * child = getChild(idx);
                if (const Type * found = child->downcast<Type>())
                {
                    return found;
                }
            }
            return 0;
        }

        virtual UInt32 componentId() const = 0;

        virtual int getWidth(SizeConstraint inSizeConstraint) const = 0;

        virtual int getHeight(SizeConstraint inSizeConstraint) const = 0;

        virtual int calculateWidth(SizeConstraint inSizeConstraint) const = 0;

        virtual int calculateHeight(SizeConstraint inSizeConstraint) const = 0;

        // Tendency to expand, used for separators, scrollbars, etc..
        virtual bool expansive() const = 0;

        virtual void move(int x, int y, int w, int h) = 0;

        virtual void move(const Rect & inRect) = 0;

        virtual Rect clientRect() const = 0;

        virtual void setOwningElement(Element * inElement) = 0;

        virtual Element * el() const = 0;

        virtual Component * parent() const = 0;

        virtual void rebuildLayout() = 0;

        virtual void rebuildChildLayouts() = 0;

        virtual LRESULT handleMessage(UINT inMessage, WPARAM wParam, LPARAM lParam) = 0;

        virtual bool getAttribute(const std::string & inName, std::string & outValue) = 0;

        virtual bool getStyle(const std::string & inName, std::string & outValue) = 0;

        virtual bool setStyle(const std::string & inName, const std::string & inValue) = 0;

        virtual bool setAttribute(const std::string & inName, const std::string & inValue) = 0;

        virtual bool initAttributeControllers() = 0;

        virtual bool initStyleControllers() = 0;
    };


} // namespace XULWin


#endif // COMPONENT_H_INCLUDED
