#include "XULWin/Decorator.h"
#include "XULWin/ChromeURL.h"
#include "XULWin/Defaults.h"
#include "XULWin/WinUtils.h"
#include "Poco/Path.h"
#include "Poco/UnicodeConverter.h"
#include <boost/bind.hpp>
#include <boost/noncopyable.hpp>
#include <boost/scoped_ptr.hpp>
#include <gdiplus.h>


namespace XULWin
{
    using namespace Windows;

    Decorator::Decorator(ElementImpl * inDecoratedElement) :
        mDecoratedElement(inDecoratedElement)
    {
        assert(mDecoratedElement);
    }


    Decorator::Decorator(ElementImplPtr inDecoratedElement) :
        mDecoratedElement(inDecoratedElement)
    {
        assert(mDecoratedElement);
    }


    Decorator::~Decorator()
    {
    }


    bool Decorator::initImpl()
    {
        assert(mDecoratedElement);
        if (mDecoratedElement)
        {
            return mDecoratedElement->initImpl();
        }
        return false;
    }


    int Decorator::getIndex() const
    {
        assert(mDecoratedElement);
        if (mDecoratedElement)
        {
            return mDecoratedElement->getIndex();
        }
        return 0;
    }


    size_t Decorator::getChildCount() const
    {
        assert(mDecoratedElement);
        if (mDecoratedElement)
        {
            return mDecoratedElement->getChildCount();
        }
        return 0;
    }


    const ElementImpl * Decorator::getChild(size_t inIndex) const
    {
        assert(mDecoratedElement);
        if (mDecoratedElement)
        {
            return mDecoratedElement->getChild(inIndex);
        }
        return 0;
    }


    ElementImpl * Decorator::getChild(size_t inIndex)
    {
        assert(mDecoratedElement);
        if (mDecoratedElement)
        {
            return mDecoratedElement->getChild(inIndex);
        }
        return 0;
    }


    HWND Decorator::getFirstParentHandle()
    {
        assert(mDecoratedElement);
        if (mDecoratedElement)
        {
            return mDecoratedElement->getFirstParentHandle();
        }
        return 0;
    }


    int Decorator::getWidth() const
    {
        assert(mDecoratedElement);
        if (mDecoratedElement)
        {
            return mDecoratedElement->getWidth();
        }
        return 0;
    }


    void Decorator::setWidth(int inWidth)
    {
        assert(mDecoratedElement);
        if (mDecoratedElement)
        {
            mDecoratedElement->setWidth(inWidth);
        }
    }


    int Decorator::getHeight() const
    {
        assert(mDecoratedElement);
        if (mDecoratedElement)
        {
            return mDecoratedElement->getHeight();
        }
        return 0;
    }


    void Decorator::setHeight(int inHeight)
    {
        assert(mDecoratedElement);
        if (mDecoratedElement)
        {
            mDecoratedElement->setHeight(inHeight);
        }
    }


    void Decorator::setStroke(const RGBColor & inColor)
    {
        assert(mDecoratedElement);
        if (mDecoratedElement)
        {
            mDecoratedElement->setStroke(inColor);
        }
    }


    const RGBColor & Decorator::getStroke() const
    {
        assert(mDecoratedElement);
        if (mDecoratedElement)
        {
            return mDecoratedElement->getStroke();
        }
        static RGBColor fDefault;
        return fDefault;
    }
    

    void Decorator::setStrokeWidth(int inStrokeWidth)
    {
        assert(mDecoratedElement);
        if (mDecoratedElement)
        {
            return mDecoratedElement->setStrokeWidth(inStrokeWidth);
        }
    }

    
    int Decorator::getStrokeWidth() const
    {
        assert(mDecoratedElement);
        if (mDecoratedElement)
        {
            return mDecoratedElement->getStrokeWidth();
        }
        return 0;
    }


    void Decorator::setFill(const RGBColor & inColor)
    {
        assert(mDecoratedElement);
        if (mDecoratedElement)
        {
            mDecoratedElement->setFill(inColor);
        }
    }


    const RGBColor & Decorator::getFill() const
    {
        assert(mDecoratedElement);
        if (mDecoratedElement)
        {
            mDecoratedElement->getFill();
        }
        static RGBColor fDefault;
        return fDefault;
    }


    int Decorator::getFlex() const
    {
        assert(mDecoratedElement);
        if (mDecoratedElement)
        {
            return mDecoratedElement->getFlex();
        }
        return 0;
    }


    void Decorator::setFlex(int inFlex)
    {
        assert(mDecoratedElement);
        if (mDecoratedElement)
        {
            mDecoratedElement->setFlex(inFlex);
        }
    }

    
    int Decorator::getCSSWidth() const
    {
        assert(mDecoratedElement);
        if (mDecoratedElement)
        {
            return mDecoratedElement->getCSSWidth();
        }
        return 0;
    }


    void Decorator::setCSSWidth(int inWidth)
    {
        assert(mDecoratedElement);
        if (mDecoratedElement)
        {
            mDecoratedElement->setCSSWidth(inWidth);
        }
    }

    
    int Decorator::getCSSHeight() const
    {
        assert(mDecoratedElement);
        if (mDecoratedElement)
        {
            return mDecoratedElement->getCSSHeight();
        }
        return 0;
    }


    void Decorator::setCSSHeight(int inHeight)
    {
        assert(mDecoratedElement);
        if (mDecoratedElement)
        {
            mDecoratedElement->setCSSHeight(inHeight);
        }
    }

    
    void Decorator::getCSSMargin(int & outTop, int & outLeft, int & outRight, int & outBottom) const
    {
        assert(mDecoratedElement);
        if (mDecoratedElement)
        {
            return mDecoratedElement->getCSSMargin(outTop, outLeft, outRight, outBottom);
        }
    }


    void Decorator::setCSSMargin(int inTop, int inLeft, int inRight, int inBottom)
    {
        assert(mDecoratedElement);
        if (mDecoratedElement)
        {
            mDecoratedElement->setCSSMargin(inTop, inLeft, inRight, inBottom);
        }
    }


    void Decorator::setCSSFill(const RGBColor & inColor)
    {
        assert(mDecoratedElement);
        if (mDecoratedElement)
        {
            mDecoratedElement->setCSSFill(inColor);
        }
    }


    const RGBColor & Decorator::getCSSFill() const
    {
        assert(mDecoratedElement);
        if (mDecoratedElement)
        {
            mDecoratedElement->getCSSFill();
        }
        static RGBColor fDefault;
        return fDefault;
    }


    void Decorator::setCSSStroke(const RGBColor & inColor)
    {
        assert(mDecoratedElement);
        if (mDecoratedElement)
        {
            mDecoratedElement->setCSSStroke(inColor);
        }
    }


    const RGBColor & Decorator::getCSSStroke() const
    {
        assert(mDecoratedElement);
        if (mDecoratedElement)
        {
            mDecoratedElement->getCSSStroke();
        }
        static RGBColor fDefault;
        return fDefault;
    }


    int Decorator::getCSSX() const
    {
        assert(mDecoratedElement);
        if (mDecoratedElement)
        {
            mDecoratedElement->getCSSX();
        }
        return 0;
    }


    void Decorator::setCSSX(int inX)
    {
        assert(mDecoratedElement);
        if (mDecoratedElement)
        {
            mDecoratedElement->setCSSX(inX);
        }
    }


    int Decorator::getCSSY() const
    {
        assert(mDecoratedElement);
        if (mDecoratedElement)
        {
            mDecoratedElement->getCSSY();
        }
        return 0;
    }


    void Decorator::setCSSY(int inY)
    {
        assert(mDecoratedElement);
        if (mDecoratedElement)
        {
            mDecoratedElement->setCSSY(inY);
        }
    }


    bool Decorator::isHidden() const
    {
        assert(mDecoratedElement);
        if (mDecoratedElement)
        {
            return mDecoratedElement->isHidden();
        }
        return false;
    }


    void Decorator::setHidden(bool inHidden)
    {
        assert(mDecoratedElement);
        if (mDecoratedElement)
        {
            return mDecoratedElement->setHidden(inHidden);
        }
    }

    
    Orient Decorator::getOrient() const
    {
        assert(mDecoratedElement);
        if (mDecoratedElement)
        {
            return mDecoratedElement->getOrient();
        }
        return Vertical;
    }


    void Decorator::setOrient(Orient inOrient)
    {
        assert(mDecoratedElement);
        if (mDecoratedElement)
        {
            return mDecoratedElement->setOrient(inOrient);
        }
    }


    Align Decorator::getAlign() const
    {
        assert(mDecoratedElement);
        if (mDecoratedElement)
        {
            return mDecoratedElement->getAlign();
        }
        return Start;
    }


    void Decorator::setAlign(Align inAlign)
    {
        assert(mDecoratedElement);
        if (mDecoratedElement)
        {
            return mDecoratedElement->setAlign(inAlign);
        }
    }


    int Decorator::commandId() const
    {
        assert(mDecoratedElement);
        if (mDecoratedElement)
        {
            return mDecoratedElement->commandId();
        }
        return 0;
    }


    int Decorator::getWidth(SizeConstraint inSizeConstraint) const
    {
        assert(mDecoratedElement);
        if (mDecoratedElement)
        {
            return mDecoratedElement->getWidth(inSizeConstraint);
        }
        return 0;
    }


    int Decorator::getHeight(SizeConstraint inSizeConstraint) const
    {
        assert(mDecoratedElement);
        if (mDecoratedElement)
        {
            return mDecoratedElement->getHeight(inSizeConstraint);
        }
        return 0;
    }


    int Decorator::calculateWidth(SizeConstraint inSizeConstraint) const
    {
        assert(mDecoratedElement);
        if (mDecoratedElement)
        {
            return mDecoratedElement->calculateWidth(inSizeConstraint);
        }
        return 0;
    }


    int Decorator::calculateHeight(SizeConstraint inSizeConstraint) const
    {
        assert(mDecoratedElement);
        if (mDecoratedElement)
        {
            return mDecoratedElement->calculateHeight(inSizeConstraint);
        }
        return 0;
    }

    
    bool Decorator::expansive() const
    {
        assert(mDecoratedElement);
        if (mDecoratedElement)
        {
            return mDecoratedElement->expansive();
        }
        return false;
    }


    void Decorator::move(int x, int y, int w, int h)
    {
        assert(mDecoratedElement);
        if (mDecoratedElement)
        {
            mDecoratedElement->move(x, y, w, h);
        }
    }
    
    
    void Decorator::move(const Rect & inRect)
    {
        assert(mDecoratedElement);
        if (mDecoratedElement)
        {
            mDecoratedElement->move(inRect);
        }
    }


    Rect Decorator::clientRect() const
    {
        assert(mDecoratedElement);
        if (mDecoratedElement)
        {
            return mDecoratedElement->clientRect();
        }
        Rect defaultRect;
        return defaultRect;
    }


    void Decorator::setOwningElement(Element * inElement)
    {
        assert(mDecoratedElement);
        if (mDecoratedElement)
        {
            mDecoratedElement->setOwningElement(inElement);
        }
    }


    Element * Decorator::el() const
    {
        assert(mDecoratedElement);
        if (mDecoratedElement)
        {
            return mDecoratedElement->el();
        }
        return 0;
    }


    ElementImpl * Decorator::parent() const
    {
        assert(mDecoratedElement);
        if (mDecoratedElement)
        {
            return mDecoratedElement->parent();
        }
        return 0;
    }


    void Decorator::rebuildLayout()
    {
        assert(mDecoratedElement);
        if (mDecoratedElement)
        {
            mDecoratedElement->rebuildLayout();
        }
    }


    void Decorator::rebuildChildLayouts()
    {
        assert(mDecoratedElement);
        if (mDecoratedElement)
        {
            mDecoratedElement->rebuildChildLayouts();
        }
    }


    bool Decorator::getAttribute(const std::string & inName, std::string & outValue)
    {
        assert(mDecoratedElement);
        if (mDecoratedElement)
        {
            return mDecoratedElement->getAttribute(inName, outValue);
        }
        return false;
    }


    bool Decorator::getStyle(const std::string & inName, std::string & outValue)
    {
        assert(mDecoratedElement);
        if (mDecoratedElement)
        {
            return mDecoratedElement->getStyle(inName, outValue);
        }
        return false;
    }


    bool Decorator::setStyle(const std::string & inName, const std::string & inValue)
    {
        assert(mDecoratedElement);
        if (mDecoratedElement)
        {
            return mDecoratedElement->setStyle(inName, inValue);
        }
        return false;
    }


    bool Decorator::setAttribute(const std::string & inName, const std::string & inValue)
    {
        assert(mDecoratedElement);
        if (mDecoratedElement)
        {
            return mDecoratedElement->setAttribute(inName, inValue);
        }
        return false;
    }


    bool Decorator::initAttributeControllers()
    {
        assert(mDecoratedElement);
        if (mDecoratedElement)
        {
            return mDecoratedElement->initAttributeControllers();
        }
        return false;
    }


    bool Decorator::initStyleControllers()
    {
        assert(mDecoratedElement);
        if (mDecoratedElement)
        {
            return mDecoratedElement->initStyleControllers();
        }
        return false;
    }


    void Decorator::setAttributeController(const std::string & inAttr, AttributeController * inController)
    {
        assert(mDecoratedElement);
        if (mDecoratedElement)
        {
            mDecoratedElement->setAttributeController(inAttr, inController);
        }
    }


    void Decorator::setStyleController(const std::string & inAttr, StyleController * inController)
    {
        assert(mDecoratedElement);
        if (mDecoratedElement)
        {
            mDecoratedElement->setStyleController(inAttr, inController);
        }
    }
    
    
    ElementImplPtr Decorator::decoratedElement() const
    {
        assert(mDecoratedElement);
        return mDecoratedElement;
    }
    
    
    void Decorator::setDecoratedElement(ElementImplPtr inElement)
    {
        mDecoratedElement = inElement;
        assert(mDecoratedElement);
    }
    
    
    void Decorator::setDecoratedElement(ElementImpl * inElement)
    {
        mDecoratedElement.reset(inElement);
        assert(mDecoratedElement);
    }


    void Decorator::onChildAdded(ElementImpl * inChild)
    {
        assert(mDecoratedElement);
        if (mDecoratedElement)
        {
            mDecoratedElement->onChildAdded(inChild);
        }
    }


    void Decorator::onChildRemoved(ElementImpl * inChild)
    {
        assert(mDecoratedElement);
        if (mDecoratedElement)
        {
            mDecoratedElement->onChildRemoved(inChild);
        }
    }


    void Decorator::onContentChanged()
    {
        assert(mDecoratedElement);
        if (mDecoratedElement)
        {
            mDecoratedElement->onContentChanged();
        }
    }


    LRESULT Decorator::handleMessage(UINT inMessage, WPARAM wParam, LPARAM lParam)
    {
        assert(mDecoratedElement);
        if (mDecoratedElement)
        {
            return mDecoratedElement->handleMessage(inMessage, wParam, lParam);
        }
        return FALSE;
    }


    WrapDecorator::WrapDecorator(ElementImpl * inParent, ElementImpl * inDecoratedElement) :
        Decorator(inDecoratedElement),
        mParent(inParent)
    {
    }


    void WrapDecorator::addChild(ElementPtr inChild)
    {
        mDecoratorChildren.push_back(inChild);
    }

    
    ScrollDecorator::ScrollDecorator(ElementImpl * inParent,
                                     ElementImpl * inDecoratedElement,
                                     CSSOverflow inOverflowX,
                                     CSSOverflow inOverflowY) :
        Decorator(inDecoratedElement),
        mOverflowX(inOverflowX),
        mOverflowY(inOverflowY),
        mOldHorScrollPos(0),
        mOldVerScrollPos(0)
    {
        if (mOverflowX != CSSOverflow_Hidden)
        {
            AttributesMapping attr;
            attr["orient"] = Orient2String(Horizontal);
            mHorizontalScrollbar = Scrollbar::Create(inParent->el(), attr);

            // Remove it from the parent so that it is untouched by its layout manager
            inParent->el()->removeChild(mHorizontalScrollbar.get());

            mHorizontalScrollbar->impl()->downcast<NativeScrollbar>()->setEventListener(this);
        }
        if (mOverflowY != CSSOverflow_Hidden)
        {
            AttributesMapping attr;
            attr["orient"] = Orient2String(Vertical);
            mVerticalScrollbar = Scrollbar::Create(inParent->el(), attr);

            // Remove it from the parent so that it is untouched by its layout manager
            inParent->el()->removeChild(mVerticalScrollbar.get());

            mVerticalScrollbar->impl()->downcast<NativeScrollbar>()->setEventListener(this);
        }
    }


    int ScrollDecorator::getWidth(SizeConstraint inSizeConstraint) const
    {
        if (inSizeConstraint == Minimum && mOverflowX != CSSOverflow_Hidden)
        {
            return 0;
        }

        return Super::getWidth(inSizeConstraint);
    }


    int ScrollDecorator::getHeight(SizeConstraint inSizeConstraint) const
    {
        if (inSizeConstraint == Minimum && mOverflowY != CSSOverflow_Hidden)
        {
            return 0;
        }
        return Super::getHeight(inSizeConstraint);
    }
            
            
    int ScrollDecorator::calculateWidth(SizeConstraint inSizeConstraint) const
    {
        if (inSizeConstraint == Minimum && mOverflowX != CSSOverflow_Hidden)
        {
            return 0;
        }

        int result = mDecoratedElement->getWidth(inSizeConstraint);
        if (mVerticalScrollbar && !mVerticalScrollbar->impl()->isHidden())
        {
            result += Defaults::scrollbarWidth();
        }
        return result;
    }


    int ScrollDecorator::calculateHeight(SizeConstraint inSizeConstraint) const
    {
        if (inSizeConstraint == Minimum && mOverflowY != CSSOverflow_Hidden)
        {
            return 0;
        }

        int result = mDecoratedElement->getHeight(inSizeConstraint);
        if (mHorizontalScrollbar && !mHorizontalScrollbar->impl()->isHidden())
        {
            result += Defaults::scrollbarWidth();
        }
        return result;
    }
    
    
    void ScrollDecorator::rebuildLayout()
    {
        bool refreshScroll = mOldHorScrollPos != 0 || mOldVerScrollPos != 0;

        Rect clientRect(clientRect());
        if (mOverflowX != CSSOverflow_Hidden)
        {
            mHorizontalScrollbar->impl()->move(
                clientRect.x(),
                clientRect.y() + clientRect.height(),
                clientRect.width(),
                Defaults::scrollbarWidth());
            mOldHorScrollPos = 0;
        }
        if (mOverflowY != CSSOverflow_Hidden)
        {
            mVerticalScrollbar->impl()->move(
                clientRect.x() + clientRect.width(),
                clientRect.y(),
                Defaults::scrollbarWidth(),
                clientRect.height());
            mOldVerScrollPos = 0;
        }

        Super::rebuildLayout();
        mOldHorScrollPos = 0;
        mOldVerScrollPos = 0;
        if (refreshScroll)
        {
            updateWindowScroll();
        }
    }
    
    
    void ScrollDecorator::move(int x, int y, int w, int h)
    {
        // Update page height of scroll boxes
        int newW = w;
        int newH = h;
        if (mOverflowX != CSSOverflow_Hidden)
        {
            NativeScrollbar * scrollbar = mHorizontalScrollbar->impl()->downcast<NativeScrollbar>();
            if (scrollbar)
            {
                int maxpos = Defaults::Attributes::maxpos();
                float ratio = (float)newW/(float)mDecoratedElement->getWidth(Optimal);
                int pageincrement = (int)(maxpos*ratio + 0.5);
                int curpos = getScrollPos(scrollbar->handle());
                if (ratio < 1)
                {
                    Windows::setScrollInfo(scrollbar->handle(), maxpos, pageincrement, curpos);
                    scrollbar->setHidden(mOverflowX == CSSOverflow_Hidden);
                    scrollbar->setDisabled(false);
                }
                else
                {
                    Windows::setScrollInfo(scrollbar->handle(), maxpos, pageincrement, 0);
                    scrollbar->setHidden(mOverflowX != CSSOverflow_Scroll);
                    scrollbar->setDisabled(true);
                }
                if (!scrollbar->isHidden())
                {
                    newH -= Defaults::scrollbarWidth();
                }
                scrollbar->setWidth(w - Defaults::scrollbarWidth());
            }
        }

        if (mOverflowY != CSSOverflow_Hidden)
        {
            NativeScrollbar * scrollbar = mVerticalScrollbar->impl()->downcast<NativeScrollbar>();
            if (scrollbar)
            {
                int maxpos = Defaults::Attributes::maxpos();
                float ratio = (float)newH/(float)mDecoratedElement->getHeight(Minimum);
                int pageincrement = (int)(maxpos*ratio + 0.5);
                int curpos = Windows::getScrollPos(scrollbar->handle());
                if (ratio < 1)
                {
                    Windows::setScrollInfo(scrollbar->handle(), maxpos, pageincrement, curpos);
                    scrollbar->setHidden(mOverflowY == CSSOverflow_Hidden);
                    scrollbar->setDisabled(false);
                }
                else
                {
                    scrollbar->setHidden(mOverflowY != CSSOverflow_Scroll);
                    scrollbar->setDisabled(true);
                    Windows::setScrollInfo(scrollbar->handle(), maxpos, pageincrement, 0);
                }
                if (!scrollbar->isHidden())
                {
                    newW -= Defaults::scrollbarWidth();
                }
                scrollbar->setHeight(h - Defaults::scrollbarWidth());
            }
        }
        Super::move(x, y, newW, newH);
    }


    void ScrollDecorator::updateWindowScroll()
    {
        if (mOverflowX == CSSOverflow_Hidden && mOverflowY == CSSOverflow_Hidden)
        {
            return;
        }

        if (NativeComponent * nativeComponent = mDecoratedElement->downcast<NativeComponent>())
        {
            int maxpos = Defaults::Attributes::maxpos();
            Rect clientRect(mDecoratedElement->clientRect());
            
            int newHorScrollPos = 0;
            int newVerScrollPos = 0;
            int dx = 0;
            int dy = 0;

            if (mHorizontalScrollbar)
            {
                NativeScrollbar * hscrollbar = mHorizontalScrollbar->impl()->downcast<NativeScrollbar>();
                int minHorSize = mDecoratedElement->getWidth(Minimum);
                int horScrollPos = Windows::getScrollPos(hscrollbar->handle());
                double horRatio = (double)horScrollPos/(double)Defaults::Attributes::maxpos();
                newHorScrollPos = (int)((horRatio * (double)minHorSize) + 0.5);
                dx = newHorScrollPos - mOldHorScrollPos;
            }

            if (mVerticalScrollbar)
            {
                NativeScrollbar * vscrollbar = mVerticalScrollbar->impl()->downcast<NativeScrollbar>();
                int minVerSize = mDecoratedElement->getHeight(Minimum);
                int verScrollPos = Windows::getScrollPos(vscrollbar->handle());
                double verRatio = (double)verScrollPos/(double)Defaults::Attributes::maxpos();
                newVerScrollPos = (int)((verRatio * (double)minVerSize) + 0.5);
                dy = newVerScrollPos - mOldVerScrollPos;
            }

            if (NativeComponent * nativeComponent = mDecoratedElement->downcast<NativeComponent>())
            {
                ::ScrollWindowEx(nativeComponent->handle(), -dx, -dy, 0, 0, 0, 0, SW_SCROLLCHILDREN | SW_INVALIDATE);
            }
            mOldHorScrollPos = newHorScrollPos;
            mOldVerScrollPos = newVerScrollPos;
        }
    }


    bool ScrollDecorator::curposChanged(NativeScrollbar * inSender, int inOldPos, int inNewPos)
    {
        updateWindowScroll();
        return true;
    }


    MarginDecorator::MarginDecorator(ElementImpl * inDecoratedElement) :
        Decorator(inDecoratedElement),
        mTop(2),
        mLeft(4),
        mRight(4),
        mBottom(2)
    {
    }


    MarginDecorator::MarginDecorator(ElementImplPtr inDecoratedElement) :
        Decorator(inDecoratedElement),
        mTop(2),
        mLeft(4),
        mRight(4),
        mBottom(2)
    {
    }


    MarginDecorator::~MarginDecorator()
    {
    }


    void MarginDecorator::setMargin(int top, int left, int right, int bottom)
    {
        mTop = top;
        mLeft = left;
        mRight = right;
        mBottom = bottom;
    }


    void MarginDecorator::getMargin(int & outTop, int & outLeft, int & outRight, int & outBottom) const
    {
        outTop = mTop;
        outLeft = mLeft;
        outRight = mRight;
        outBottom = mBottom;
    }


    int MarginDecorator::marginTop() const
    {
        return mTop;
    }

    
    int MarginDecorator::marginLeft() const
    {
        return mLeft;
    }

    
    int MarginDecorator::marginRight() const
    {
        return mRight;
    }

    
    int MarginDecorator::marginBottom() const
    {
        return mBottom;
    }

    
    Rect MarginDecorator::clientRect() const
    {
        return mOuterRect;
    }


    int MarginDecorator::getWidth() const
    {
        return marginLeft() + Super::getWidth() + marginRight();
    }


    void MarginDecorator::setWidth(int inWidth)
    {
        mOuterRect = Rect(mOuterRect.x(), mOuterRect.y(), inWidth, mOuterRect.height());
        Super::setWidth(inWidth);
    }


    int MarginDecorator::getHeight() const
    {
        return marginTop() + Super::getHeight() + marginBottom();
    }


    void MarginDecorator::setHeight(int inHeight)
    {
        mOuterRect = Rect(mOuterRect.x(), mOuterRect.y(), mOuterRect.width(), inHeight);
        Super::setHeight(inHeight);
    }


    int MarginDecorator::getWidth(SizeConstraint inSizeConstraint) const
    {
        return marginLeft() + Super::getWidth(inSizeConstraint) + marginRight();
    }


    int MarginDecorator::getHeight(SizeConstraint inSizeConstraint) const
    {
        return marginTop() + Super::getHeight(inSizeConstraint) + marginBottom();
    }

    
    void MarginDecorator::move(const Rect & inRect)
    {
        MarginDecorator::move(inRect.x(), inRect.y(), inRect.width(), inRect.height());
    }

    
    void MarginDecorator::move(int x, int y, int w, int h)
    {
        mOuterRect = Rect(x, y, w, h);
        mInnerRect = Rect(x + marginLeft(),
                          y + marginTop(),
                          w - marginLeft() - marginRight(),
                          h - marginTop() - marginBottom());
        if (mDecoratedElement)
        {
            mDecoratedElement->move(mInnerRect.x(),
                                    mInnerRect.y(),
                                    mInnerRect.width(),
                                    mInnerRect.height());
        }
    }
    

    int MarginDecorator::calculateWidth(SizeConstraint inSizeConstraint) const
    {
        return marginLeft() + Super::getWidth(inSizeConstraint) + marginRight();
    }

    
    int MarginDecorator::calculateHeight(SizeConstraint inSizeConstraint) const
    {
        return marginTop() + Super::getHeight(inSizeConstraint) + marginBottom();
    }

} // namespace XULWin
