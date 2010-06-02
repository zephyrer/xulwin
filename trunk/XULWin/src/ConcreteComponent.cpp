#include "XULWin/ConcreteComponent.h"
#include "XULWin/Algorithms.h"
#include "XULWin/Decorator.h"
#include "XULWin/Element.h"
#include "XULWin/ErrorReporter.h"
#include "XULWin/NativeControl.h"
#include <boost/bind.hpp>


namespace XULWin
{

    ConcreteComponent::ConcreteComponent(Component * inParent) :
        mParent(inParent),
        mExpansive(false),
        mElement(0),
        mFlex(0),
        mHidden(false),
        mOrient(Horizontal),
        mAlign(Start),
        mCSSX(0),
        mCSSY(0),
        mWidth(0),
        mHeight(0),
        mScreenX(0),
        mScreenY(0),
        mStrokeWidth(1),
        mCSSWidth(0),
        mCSSHeight(0),
        mCSSFill(RGBColor()),
        mCSSStroke(RGBColor(0, 0, 0, 0)),
        mIsInitialized(false)
    {
        mCSSX.setInvalid();
        mCSSY.setInvalid();
        mWidth.setInvalid();
        mHeight.setInvalid();
        mScreenX.setInvalid();
        mScreenY.setInvalid();
        mStrokeWidth.setInvalid();
        mCSSWidth.setInvalid();
        mCSSHeight.setInvalid();
        mCSSFill.setInvalid();
        mCSSStroke.setInvalid();
        mCSSBackgroundColor.setInvalid();
        mOrient.setInvalid();
        mAlign.setInvalid();
    }


    ConcreteComponent::~ConcreteComponent()
    {
    }


    bool ConcreteComponent::init()
    {
        mIsInitialized = true;
        return true;
    }


    int ConcreteComponent::calculateMaxChildWidth(SizeConstraint inSizeConstraint) const
    {
        return max_element_value<int>(el()->children().begin(),
                                      el()->children().end(),
                                      0,
                                      boost::bind(&Component::calculateWidth,
                                                  boost::bind(&Element::component, _1),
                                                  inSizeConstraint));
    }


    int ConcreteComponent::calculateMaxChildHeight(SizeConstraint inSizeConstraint) const
    {
        return max_element_value<int>(el()->children().begin(),
                                      el()->children().end(),
                                      0,
                                      boost::bind(&Component::calculateHeight,
                                                  boost::bind(&Element::component, _1),
                                                  inSizeConstraint));
    }


    int ConcreteComponent::calculateSumChildWidths(SizeConstraint inSizeConstraint) const
    {
        return sum_element_values<int>(el()->children().begin(),
                                       el()->children().end(),
                                       0,
                                       boost::bind(&Component::calculateWidth,
                                                   boost::bind(&Element::component, _1),
                                                   inSizeConstraint));
    }


    int ConcreteComponent::calculateSumChildHeights(SizeConstraint inSizeConstraint) const
    {
        return sum_element_values<int>(el()->children().begin(),
                                       el()->children().end(),
                                       0,
                                       boost::bind(&Component::calculateHeight,
                                                   boost::bind(&Element::component, _1),
                                                   inSizeConstraint));
    }


    int ConcreteComponent::getIndex() const
    {
        size_t idx = 0;
        for (; idx != parent()->getChildCount(); ++idx)
        {
            if (Component * child = parent()->getChild(idx))
            {
                if (this == child->downcast<ConcreteComponent>())
                {
                    return idx;
                }
            }
        }
        ReportError("Component failed to find itself in its parent's list of children.");
        return idx;
    }


    size_t ConcreteComponent::getChildCount() const
    {
        return el()->children().size();
    }


    const Component * ConcreteComponent::getChild(size_t inIndex) const
    {
        return el()->children()[inIndex]->component();
    }


    Component * ConcreteComponent::getChild(size_t inIndex)
    {
        return el()->children()[inIndex]->component();
    }


    // FIXME! This does not belong here!
    HWND ConcreteComponent::getFirstParentHandle()
    {
        if (NativeComponent * comp = NativeControl::FindNativeParent(parent()))
        {
            return comp->handle();
        }
        return 0;
    }


    void ConcreteComponent::invalidateRect() const
    {
        // Just forward to all children.
        for (size_t idx = 0; idx != getChildCount(); ++idx)
        {
            getChild(idx)->invalidateRect();
        }
    }


    const RGBColor & ConcreteComponent::getCSSFill() const
    {
        if (mCSSFill.isValid())
        {
            return mCSSFill;
        }

        if (parent())
        {
            return parent()->getCSSFill();
        }

        return mCSSFill; // default value
    }


    void ConcreteComponent::setCSSFill(const RGBColor & inColor)
    {
        mCSSFill = inColor;
    }


    void ConcreteComponent::setCSSStroke(const RGBColor & inColor)
    {
        mCSSStroke = inColor;
    }


    const RGBColor & ConcreteComponent::getCSSStroke() const
    {
        if (mCSSStroke.isValid())
        {
            return mCSSStroke.getValue();
        }

        if (parent())
        {
            return parent()->getCSSStroke();
        }

        return mCSSStroke;
    }


    int ConcreteComponent::getCSSX() const
    {
        return mCSSX;
    }


    void ConcreteComponent::setCSSX(int inX)
    {
        mCSSX = inX;
    }


    int ConcreteComponent::getCSSY() const
    {
        return mCSSY;
    }


    void ConcreteComponent::setCSSY(int inY)
    {
        mCSSY = inY;
    }


    int ConcreteComponent::getCSSWidth() const
    {
        if (mWidth.isValid())
        {
            return mWidth.getValue();
        }
        return mCSSWidth;
    }


    void ConcreteComponent::setCSSWidth(int inWidth)
    {
        mCSSWidth = inWidth;
    }


    void ConcreteComponent::setCSSBackgroundColor(const RGBColor & inColor)
    {
        mCSSBackgroundColor = inColor;
    }


    RGBColor ConcreteComponent::getCSSBackgroundColor() const
    {
        return mCSSBackgroundColor;
    }


    int ConcreteComponent::getCSSHeight() const
    {
        if (mHeight.isValid())
        {
            return mHeight.getValue();
        }
        return mCSSHeight;
    }


    void ConcreteComponent::setCSSHeight(int inHeight)
    {
        mCSSHeight = inHeight;
    }


    int ConcreteComponent::getWidth() const
    {
        if (mWidth.isValid())
        {
            return mWidth;
        }

        if (mCSSWidth.isValid())
        {
            return std::max<int>(mCSSWidth.getValue(), getWidth(Minimum));
        }
        return getWidth(Preferred);
    }


    void ConcreteComponent::setWidth(int inWidth)
    {
        mWidth = inWidth;
    }


    int ConcreteComponent::getHeight() const
    {
        if (mHeight.isValid())
        {
            return mHeight;
        }

        if (mCSSHeight.isValid())
        {
            return std::max<int>(mCSSHeight.getValue(), getHeight(Minimum));
        }

        return getHeight(Preferred);
    }


    void ConcreteComponent::setHeight(int inHeight)
    {
        mHeight = inHeight;
    }


    int ConcreteComponent::getScreenX() const
    {
        if (mScreenX.isValid())
        {
            return mScreenX.getValue();
        }

        if (mCSSX.isValid())
        {
            return mCSSX.getValue();
        }

        return 100;
    }


    void ConcreteComponent::setScreenX(int inX)
    {
        mScreenX = inX;
    }


    int ConcreteComponent::getScreenY() const
    {
        if (mScreenY.isValid())
        {
            return mScreenY.getValue();
        }

        if (mCSSY.isValid())
        {
            return mCSSY.getValue();
        }

        return 100;
    }


    void ConcreteComponent::setScreenY(int inY)
    {
        mScreenY = inY;
    }


    int ConcreteComponent::getFlex() const
    {
        return mFlex;
    }


    void ConcreteComponent::setFlex(int inFlex)
    {
        mFlex = inFlex;
    }


    bool ConcreteComponent::isHidden() const
    {
        return mHidden;
    }


    void ConcreteComponent::setHidden(bool inHidden)
    {
        mHidden = inHidden;
        for (size_t idx = 0; idx != getChildCount(); ++idx)
        {
            getChild(idx)->setHidden(inHidden);
        }
    }


    Orient ConcreteComponent::getOrient() const
    {
        return mOrient.or(Vertical);
    }


    void ConcreteComponent::setOrient(Orient inOrient)
    {
        mOrient = inOrient;
    }


    Align ConcreteComponent::getAlign() const
    {
        return mAlign.or((getOrient() == Vertical) ? Stretch : Start);
    }


    void ConcreteComponent::setAlign(Align inAlign)
    {
        mAlign = inAlign;
    }


    void ConcreteComponent::getCSSMargin(int & outTop, int & outLeft, int & outRight, int & outBottom) const
    {
        if (const MarginDecorator * marginDecorator = downcast<MarginDecorator>())
        {
            marginDecorator->getMargin(outTop, outLeft, outRight, outBottom);
        }
    }


    void ConcreteComponent::setCSSMargin(int inTop, int inLeft, int inRight, int inBottom)
    {
        if (!el())
        {
            ReportError("ConcreteComponent::setCSSMargin failed because no owning element was present.");
            return;
        }

        // Find a margin decorator, and set the margin value.
        if (MarginDecorator * obj = el()->component()->downcast<MarginDecorator>())
        {
            obj->setMargin(inTop, inLeft, inRight, inBottom);
        }
        // If no margin decorator found, insert one, and set the value.
        else if (Decorator * dec = el()->component()->downcast<Decorator>())
        {
            ComponentPtr newDec(new MarginDecorator(dec->decoratedElement()));
            dec->setDecoratedElement(newDec);
            if (MarginDecorator * p = newDec->downcast<MarginDecorator>())
            {
                p->setMargin(inTop, inLeft, inRight, inBottom);
            }
        }
        else
        {
            ReportError("No decorator found!");
        }
    }


    int ConcreteComponent::getWidth(SizeConstraint inSizeConstraint) const
    {
        if (isHidden())
        {
            return 0;
        }

        if (mWidth.isValid())
        {
            return mWidth.getValue();
        }

        return calculateWidth(inSizeConstraint);
    }


    int ConcreteComponent::getHeight(SizeConstraint inSizeConstraint) const
    {
        if (isHidden())
        {
            return 0;
        }

        if (mHeight.isValid())
        {
            return mHeight.getValue();
        }

        return calculateHeight(inSizeConstraint);
    }


    bool ConcreteComponent::expansive() const
    {
        return mExpansive;
    }


    bool ConcreteComponent::getStyle(const std::string & inName, std::string & outValue)
    {
        StyleControllers::iterator it = mStyleControllers.find(inName);
        if (it != mStyleControllers.end())
        {
            StyleController * controller = it->second;
            controller->get(outValue);
        }
        return false;
    }


    bool ConcreteComponent::getAttribute(const std::string & inName, std::string & outValue)
    {
        AttributeControllers::iterator it = mAttributeControllers.find(inName);
        if (it != mAttributeControllers.end())
        {
            it->second->get(outValue);
            return true;
        }
        return false;
    }


    bool ConcreteComponent::setStyle(const std::string & inName, const std::string & inValue)
    {
        StyleControllers::iterator it = mStyleControllers.find(inName);
        if (it != mStyleControllers.end())
        {
            ErrorCatcher errorCatcher;
            StyleController * controller = it->second;
            controller->set(inValue);
            if (errorCatcher.hasCaught())
            {
                // FIXME! Do meaningful error handling!
                //ReportSetterFail(inName, el()->tagName(), el()->getAttribute("id"));
            }
            return true;
        }
        return false;
    }


    bool ConcreteComponent::setAttribute(const std::string & inName, const std::string & inValue)
    {
        AttributeControllers::iterator it = mAttributeControllers.find(inName);
        if (it != mAttributeControllers.end())
        {
            ErrorCatcher errorCatcher;
            it->second->set(inValue);
            if (errorCatcher.hasCaught())
            {
                //FIXME! Do meaningful error handling here!
                //ReportSetterFail(inName, el()->tagName(), el()->getAttribute("id"));
            }
            return true;
        }
        return false;
    }


    bool ConcreteComponent::initAttributeControllers()
    {
        setAttributeController<WidthController>(this);
        setAttributeController<HeightController>(this);
        setAttributeController<FlexController>(this);
        setAttributeController<HiddenController>(this);
        setAttributeController<AlignController>(this);
        setAttributeController<OrientController>(this);
        return true;
    }


    bool ConcreteComponent::initStyleControllers()
    {
        setStyleController<CSSXController>(this);
        setStyleController<CSSYController>(this);
        setStyleController<CSSWidthController>(this);
        setStyleController<CSSHeightController>(this);
        setStyleController<CSSMarginController>(this);
        setStyleController<CSSBackgroundColorController>(this);
        setStyleController<CSSFillController>(this);
        setStyleController<CSSStrokeController>(this);
        return true;
    }


    void ConcreteComponent::move(const Rect & inRect)
    {
        move(inRect.x(), inRect.y(), inRect.width(), inRect.height());
    }


    void ConcreteComponent::setOwningElement(Element * inElement)
    {
        mElement = inElement;
    }


    Element * ConcreteComponent::el() const
    {
        return mElement;
    }


    Component * ConcreteComponent::parent() const
    {
        return mParent;
    }


    void ConcreteComponent::rebuildChildLayouts()
    {
        if (!mElement)
        {
            return;
        }

        for (size_t idx = 0; idx != mElement->children().size(); ++idx)
        {
            Component * nativeComp = mElement->children()[idx]->component();
            if (nativeComp)
            {
                nativeComp->rebuildLayout();
            }
        }
    }

} // namespace XULWin
