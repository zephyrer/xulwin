#include "XULWin/Decorator.h"
#include "XULWin/ComponentFactory.h"
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

    Decorator::Decorator(Component * inDecoratedElement) :
        mDecoratedComponent(inDecoratedElement)
    {
        assert(mDecoratedComponent);
    }


    Decorator::Decorator(ComponentPtr inDecoratedElement) :
        mDecoratedComponent(inDecoratedElement)
    {
        assert(mDecoratedComponent);
    }


    Decorator::~Decorator()
    {
    }


    bool Decorator::init()
    {
        assert(mDecoratedComponent);
        if (mDecoratedComponent)
        {
            return mDecoratedComponent->init();
        }
        return false;
    }

    //int Decorator::getIndex() const
    //{
    //    assert(mDecoratedComponent);
    //    if (mDecoratedComponent)
    //    {
    //        return mDecoratedComponent->getIndex();
    //    }
    //    return 0;
    //}
    
    
    void Decorator::addChild(ComponentPtr inComponent)
    {
        assert(mDecoratedComponent);
        if (mDecoratedComponent)
        {
            mDecoratedComponent->addChild(inComponent);
        }
    }


    size_t Decorator::getChildCount() const
    {
        assert(mDecoratedComponent);
        if (mDecoratedComponent)
        {
            return mDecoratedComponent->getChildCount();
        }
        return 0;
    }


    const Component * Decorator::getChild(size_t inIndex) const
    {
        assert(mDecoratedComponent);
        if (mDecoratedComponent)
        {
            return mDecoratedComponent->getChild(inIndex);
        }
        return 0;
    }


    Component * Decorator::getChild(size_t inIndex)
    {
        assert(mDecoratedComponent);
        if (mDecoratedComponent)
        {
            return mDecoratedComponent->getChild(inIndex);
        }
        return 0;
    }


    HWND Decorator::getFirstParentHandle()
    {
        assert(mDecoratedComponent);
        if (mDecoratedComponent)
        {
            return mDecoratedComponent->getFirstParentHandle();
        }
        return 0;
    }


    void Decorator::invalidateRect() const
    {
        assert(mDecoratedComponent);
        if (mDecoratedComponent)
        {
            mDecoratedComponent->invalidateRect();
        }
    }


    int Decorator::getWidth() const
    {
        assert(mDecoratedComponent);
        if (mDecoratedComponent)
        {
            return mDecoratedComponent->getWidth();
        }
        return 0;
    }


    void Decorator::setWidth(int inWidth)
    {
        assert(mDecoratedComponent);
        if (mDecoratedComponent)
        {
            mDecoratedComponent->setWidth(inWidth);
        }
    }


    int Decorator::getHeight() const
    {
        assert(mDecoratedComponent);
        if (mDecoratedComponent)
        {
            return mDecoratedComponent->getHeight();
        }
        return 0;
    }


    void Decorator::setHeight(int inHeight)
    {
        assert(mDecoratedComponent);
        if (mDecoratedComponent)
        {
            mDecoratedComponent->setHeight(inHeight);
        }
    }


    int Decorator::getScreenX() const
    {
        assert(mDecoratedComponent);
        if (mDecoratedComponent)
        {
            return mDecoratedComponent->getScreenX();
        }
        return 0;
    }


    void Decorator::setScreenX(int inX)
    {
        assert(mDecoratedComponent);
        if (mDecoratedComponent)
        {
            mDecoratedComponent->setScreenX(inX);
        }
    }


    int Decorator::getScreenY() const
    {
        assert(mDecoratedComponent);
        if (mDecoratedComponent)
        {
            return mDecoratedComponent->getScreenY();
        }
        return 0;
    }


    void Decorator::setScreenY(int inY)
    {
        assert(mDecoratedComponent);
        if (mDecoratedComponent)
        {
            mDecoratedComponent->setScreenY(inY);
        }
    }


    void Decorator::setSVGStroke(const RGBColor & inColor)
    {
        assert(false); // TODO: remove this method
        //assert(mDecoratedComponent);
        //if (mDecoratedComponent)
        //{
        //    mDecoratedComponent->setSVGStroke(inColor);
        //}
    }


    const RGBColor & Decorator::getSVGStroke() const
    {
        assert(false); // TODO: remove this method
        //assert(mDecoratedComponent);
        //if (mDecoratedComponent)
        //{
        //    return mDecoratedComponent->getSVGStroke();
        //}
        static RGBColor fDefault;
        return fDefault;
    }


    void Decorator::setSVGStrokeWidth(int inStrokeWidth)
    {
        assert(false); // TODO: remove this method
        //assert(mDecoratedComponent);
        //if (mDecoratedComponent)
        //{
        //    return mDecoratedComponent->setSVGStrokeWidth(inStrokeWidth);
        //}
    }


    int Decorator::getSVGStrokeWidth() const
    {
        assert(false); // TODO: remove this method
        //assert(mDecoratedComponent);
        //if (mDecoratedComponent)
        //{
        //    return mDecoratedComponent->getSVGStrokeWidth();
        //}
        return 0;
    }


    void Decorator::setSVGFill(const RGBColor & inColor)
    {
        assert(false); // TODO: remove this method
        //assert(mDecoratedComponent);
        //if (mDecoratedComponent)
        //{
        //    mDecoratedComponent->setSVGFill(inColor);
        //}
    }


    const RGBColor & Decorator::getSVGFill() const
    {
        assert(false); // TODO: remove this method
        //assert(mDecoratedComponent);
        //if (mDecoratedComponent)
        //{
        //    mDecoratedComponent->getSVGFill();
        //}
        static RGBColor fDefault;
        return fDefault;
    }


    int Decorator::getFlex() const
    {
        assert(mDecoratedComponent);
        if (mDecoratedComponent)
        {
            return mDecoratedComponent->getFlex();
        }
        return 0;
    }


    void Decorator::setFlex(int inFlex)
    {
        assert(mDecoratedComponent);
        if (mDecoratedComponent)
        {
            mDecoratedComponent->setFlex(inFlex);
        }
    }


    int Decorator::getCSSWidth() const
    {
        assert(mDecoratedComponent);
        if (mDecoratedComponent)
        {
            return mDecoratedComponent->getCSSWidth();
        }
        return 0;
    }


    void Decorator::setCSSWidth(int inWidth)
    {
        assert(mDecoratedComponent);
        if (mDecoratedComponent)
        {
            mDecoratedComponent->setCSSWidth(inWidth);
        }
    }


    int Decorator::getCSSHeight() const
    {
        assert(mDecoratedComponent);
        if (mDecoratedComponent)
        {
            return mDecoratedComponent->getCSSHeight();
        }
        return 0;
    }


    void Decorator::setCSSHeight(int inHeight)
    {
        assert(mDecoratedComponent);
        if (mDecoratedComponent)
        {
            mDecoratedComponent->setCSSHeight(inHeight);
        }
    }


    void Decorator::setCSSBackgroundColor(const RGBColor & inColor)
    {
        assert(mDecoratedComponent);
        if (mDecoratedComponent)
        {
            mDecoratedComponent->setCSSBackgroundColor(inColor);
        }
    }


    RGBColor Decorator::getCSSBackgroundColor() const
    {
        assert(mDecoratedComponent);
        if (mDecoratedComponent)
        {
            return mDecoratedComponent->getCSSBackgroundColor();
        }
        RGBColor fallback;
        (0, 0, 0);
        return fallback;
    }


    void Decorator::getCSSMargin(int & outTop, int & outLeft, int & outRight, int & outBottom) const
    {
        assert(mDecoratedComponent);
        if (mDecoratedComponent)
        {
            return mDecoratedComponent->getCSSMargin(outTop, outLeft, outRight, outBottom);
        }
    }


    void Decorator::setCSSMargin(int inTop, int inLeft, int inRight, int inBottom)
    {
        assert(mDecoratedComponent);
        if (mDecoratedComponent)
        {
            mDecoratedComponent->setCSSMargin(inTop, inLeft, inRight, inBottom);
        }
    }


    void Decorator::setCSSFill(const RGBColor & inColor)
    {
        assert(mDecoratedComponent);
        if (mDecoratedComponent)
        {
            mDecoratedComponent->setCSSFill(inColor);
        }
    }


    const RGBColor & Decorator::getCSSFill() const
    {
        assert(mDecoratedComponent);
        if (mDecoratedComponent)
        {
            mDecoratedComponent->getCSSFill();
        }
        static RGBColor fDefault;
        return fDefault;
    }


    void Decorator::setCSSStroke(const RGBColor & inColor)
    {
        assert(mDecoratedComponent);
        if (mDecoratedComponent)
        {
            mDecoratedComponent->setCSSStroke(inColor);
        }
    }


    const RGBColor & Decorator::getCSSStroke() const
    {
        assert(mDecoratedComponent);
        if (mDecoratedComponent)
        {
            mDecoratedComponent->getCSSStroke();
        }
        static RGBColor fDefault;
        return fDefault;
    }


    int Decorator::getCSSX() const
    {
        assert(mDecoratedComponent);
        if (mDecoratedComponent)
        {
            mDecoratedComponent->getCSSX();
        }
        return 0;
    }


    void Decorator::setCSSX(int inX)
    {
        assert(mDecoratedComponent);
        if (mDecoratedComponent)
        {
            mDecoratedComponent->setCSSX(inX);
        }
    }


    int Decorator::getCSSY() const
    {
        assert(mDecoratedComponent);
        if (mDecoratedComponent)
        {
            mDecoratedComponent->getCSSY();
        }
        return 0;
    }


    void Decorator::setCSSY(int inY)
    {
        assert(mDecoratedComponent);
        if (mDecoratedComponent)
        {
            mDecoratedComponent->setCSSY(inY);
        }
    }


    bool Decorator::isHidden() const
    {
        assert(mDecoratedComponent);
        if (mDecoratedComponent)
        {
            return mDecoratedComponent->isHidden();
        }
        return false;
    }


    void Decorator::setHidden(bool inHidden)
    {
        assert(mDecoratedComponent);
        if (mDecoratedComponent)
        {
            return mDecoratedComponent->setHidden(inHidden);
        }
    }


    Orient Decorator::getOrient() const
    {
        assert(mDecoratedComponent);
        if (mDecoratedComponent)
        {
            return mDecoratedComponent->getOrient();
        }
        return Vertical;
    }


    void Decorator::setOrient(Orient inOrient)
    {
        assert(mDecoratedComponent);
        if (mDecoratedComponent)
        {
            return mDecoratedComponent->setOrient(inOrient);
        }
    }


    Align Decorator::getAlign() const
    {
        assert(mDecoratedComponent);
        if (mDecoratedComponent)
        {
            return mDecoratedComponent->getAlign();
        }
        return Start;
    }


    void Decorator::setAlign(Align inAlign)
    {
        assert(mDecoratedComponent);
        if (mDecoratedComponent)
        {
            return mDecoratedComponent->setAlign(inAlign);
        }
    }


    int Decorator::commandId() const
    {
        assert(mDecoratedComponent);
        if (mDecoratedComponent)
        {
            return mDecoratedComponent->commandId();
        }
        return 0;
    }


    int Decorator::getWidth(SizeConstraint inSizeConstraint) const
    {
        assert(mDecoratedComponent);
        if (mDecoratedComponent)
        {
            return mDecoratedComponent->getWidth(inSizeConstraint);
        }
        return 0;
    }


    int Decorator::getHeight(SizeConstraint inSizeConstraint) const
    {
        assert(mDecoratedComponent);
        if (mDecoratedComponent)
        {
            return mDecoratedComponent->getHeight(inSizeConstraint);
        }
        return 0;
    }


    int Decorator::calculateWidth(SizeConstraint inSizeConstraint) const
    {
        assert(mDecoratedComponent);
        if (mDecoratedComponent)
        {
            return mDecoratedComponent->calculateWidth(inSizeConstraint);
        }
        return 0;
    }


    int Decorator::calculateHeight(SizeConstraint inSizeConstraint) const
    {
        assert(mDecoratedComponent);
        if (mDecoratedComponent)
        {
            return mDecoratedComponent->calculateHeight(inSizeConstraint);
        }
        return 0;
    }


    bool Decorator::expansive() const
    {
        assert(mDecoratedComponent);
        if (mDecoratedComponent)
        {
            return mDecoratedComponent->expansive();
        }
        return false;
    }


    void Decorator::move(int x, int y, int w, int h)
    {
        assert(mDecoratedComponent);
        if (mDecoratedComponent)
        {
            mDecoratedComponent->move(x, y, w, h);
        }
    }


    void Decorator::move(const Rect & inRect)
    {
        assert(mDecoratedComponent);
        if (mDecoratedComponent)
        {
            mDecoratedComponent->move(inRect);
        }
    }


    Rect Decorator::clientRect() const
    {
        assert(mDecoratedComponent);
        if (mDecoratedComponent)
        {
            return mDecoratedComponent->clientRect();
        }
        Rect defaultRect;
        return defaultRect;
    }


    void Decorator::setOwningElement(Poco::XML::Element * inElement)
    {
        assert(mDecoratedComponent);
        if (mDecoratedComponent)
        {
            mDecoratedComponent->setOwningElement(inElement);
        }
    }


    Poco::XML::Element * Decorator::el() const
    {
        assert(mDecoratedComponent);
        if (mDecoratedComponent)
        {
            return mDecoratedComponent->el();
        }
        return 0;
    }


    Component * Decorator::parentComponent() const
    {
        assert(mDecoratedComponent);
        if (mDecoratedComponent)
        {
            return mDecoratedComponent->parentComponent();
        }
        return 0;
    }


    void Decorator::rebuildLayout()
    {
        assert(mDecoratedComponent);
        if (mDecoratedComponent)
        {
            mDecoratedComponent->rebuildLayout();
        }
    }


    void Decorator::rebuildChildLayouts()
    {
        assert(mDecoratedComponent);
        if (mDecoratedComponent)
        {
            mDecoratedComponent->rebuildChildLayouts();
        }
    }


    bool Decorator::getAttribute(const std::string & inName, std::string & outValue)
    {
        assert(mDecoratedComponent);
        if (mDecoratedComponent)
        {
            return mDecoratedComponent->getAttribute(inName, outValue);
        }
        return false;
    }


    bool Decorator::getStyle(const std::string & inName, std::string & outValue)
    {
        assert(mDecoratedComponent);
        if (mDecoratedComponent)
        {
            return mDecoratedComponent->getStyle(inName, outValue);
        }
        return false;
    }


    bool Decorator::setStyle(const std::string & inName, const std::string & inValue)
    {
        assert(mDecoratedComponent);
        if (mDecoratedComponent)
        {
            return mDecoratedComponent->setStyle(inName, inValue);
        }
        return false;
    }


    bool Decorator::setAttribute(const std::string & inName, const std::string & inValue)
    {
        assert(mDecoratedComponent);
        if (mDecoratedComponent)
        {
            return mDecoratedComponent->setAttribute(inName, inValue);
        }
        return false;
    }


    bool Decorator::initAttributeControllers()
    {
        assert(mDecoratedComponent);
        if (mDecoratedComponent)
        {
            return mDecoratedComponent->initAttributeControllers();
        }
        return false;
    }


    bool Decorator::initStyleControllers()
    {
        assert(mDecoratedComponent);
        if (mDecoratedComponent)
        {
            return mDecoratedComponent->initStyleControllers();
        }
        return false;
    }


    ComponentPtr Decorator::decoratedComponent() const
    {
        assert(mDecoratedComponent);
        return mDecoratedComponent;
    }


    void Decorator::setDecoratedElement(ComponentPtr inElement)
    {
        mDecoratedComponent = inElement;
        assert(mDecoratedComponent);
    }


    void Decorator::setDecoratedElement(Component * inElement)
    {
        mDecoratedComponent.reset(inElement);
        assert(mDecoratedComponent);
    }


    void Decorator::onChildAdded(Component * inChild)
    {
        assert(mDecoratedComponent);
        if (mDecoratedComponent)
        {
            mDecoratedComponent->onChildAdded(inChild);
        }
    }


    void Decorator::onChildRemoved(Component * inChild)
    {
        assert(mDecoratedComponent);
        if (mDecoratedComponent)
        {
            mDecoratedComponent->onChildRemoved(inChild);
        }
    }


    void Decorator::onContentChanged()
    {
        assert(mDecoratedComponent);
        if (mDecoratedComponent)
        {
            mDecoratedComponent->onContentChanged();
        }
    }


    LRESULT Decorator::handleMessage(UINT inMessage, WPARAM wParam, LPARAM lParam)
    {
        assert(mDecoratedComponent);
        if (mDecoratedComponent)
        {
            return mDecoratedComponent->handleMessage(inMessage, wParam, lParam);
        }
        return FALSE;
    }


    //ScrollDecorator::ScrollDecorator(Component * inParent,
    //                                 Component * inDecoratedComponent,
    //                                 CSSOverflow inOverflowX,
    //                                 CSSOverflow inOverflowY) :
    //    Decorator(inDecoratedComponent),
    //    mOverflowX(inOverflowX),
    //    mOverflowY(inOverflowY),
    //    mOldHorScrollPos(0),
    //    mOldVerScrollPos(0)
    //{
    //    if (mOverflowX != CSSOverflow_Hidden)
    //    {
    //        AttributesMapping attr;
    //        attr["orient"] = Orient2String(Horizontal);

    //        //Component * comp = CreateDecoratedComponent<Decorator, Scrollbar>(inParent, attr);
    //        //mHorizontalScrollbar.reset(comp->downcast<Scrollbar>());
    //        //mHorizontalScrollbar->downcast<Scrollbar>()->setEventHandler(this);
    //    }
    //    if (mOverflowY != CSSOverflow_Hidden)
    //    {
    //        AttributesMapping attr;
    //        attr["orient"] = Orient2String(Vertical);

    //        //Component * comp = CreateDecoratedComponent<Decorator, Scrollbar>(inParent, attr);
    //        //mVerticalScrollbar.reset(comp->downcast<Scrollbar>());
    //        //mVerticalScrollbar->downcast<Scrollbar>()->setEventHandler(this);
    //    }
    //}


    //bool ScrollDecorator::init()
    //{
    //    // TODO: enable mousewheel
    //    return Super::init();
    //}


    //int ScrollDecorator::getWidth(SizeConstraint inSizeConstraint) const
    //{
    //    if (inSizeConstraint == Minimum && mOverflowX != CSSOverflow_Hidden)
    //    {
    //        return 0;
    //    }

    //    return Super::getWidth(inSizeConstraint);
    //}


    //int ScrollDecorator::getHeight(SizeConstraint inSizeConstraint) const
    //{
    //    if (inSizeConstraint == Minimum && mOverflowY != CSSOverflow_Hidden)
    //    {
    //        return 0;
    //    }
    //    return Super::getHeight(inSizeConstraint);
    //}


    //int ScrollDecorator::calculateWidth(SizeConstraint inSizeConstraint) const
    //{
    //    if (inSizeConstraint == Minimum && mOverflowX != CSSOverflow_Hidden)
    //    {
    //        return 0;
    //    }

    //    int result = mDecoratedComponent->getWidth(inSizeConstraint);
    //    if (mVerticalScrollbar && !mVerticalScrollbar->isHidden())
    //    {
    //        result += Defaults::scrollbarWidth();
    //    }
    //    return result;
    //}


    //int ScrollDecorator::calculateHeight(SizeConstraint inSizeConstraint) const
    //{
    //    if (inSizeConstraint == Minimum && mOverflowY != CSSOverflow_Hidden)
    //    {
    //        return 0;
    //    }

    //    int result = mDecoratedComponent->getHeight(inSizeConstraint);
    //    if (mHorizontalScrollbar && !mHorizontalScrollbar->isHidden())
    //    {
    //        result += Defaults::scrollbarWidth();
    //    }
    //    return result;
    //}


    //void ScrollDecorator::rebuildLayout()
    //{
    //    bool refreshScroll = mOldHorScrollPos != 0 || mOldVerScrollPos != 0;

    //    Rect clientRect(clientRect());
    //    if (mOverflowX != CSSOverflow_Hidden)
    //    {
    //        mHorizontalScrollbar->move(
    //            clientRect.x(),
    //            clientRect.y() + clientRect.height(),
    //            clientRect.width(),
    //            Defaults::scrollbarWidth());
    //        mOldHorScrollPos = 0;
    //    }
    //    if (mOverflowY != CSSOverflow_Hidden)
    //    {
    //        mVerticalScrollbar->move(
    //            clientRect.x() + clientRect.width(),
    //            clientRect.y(),
    //            Defaults::scrollbarWidth(),
    //            clientRect.height());
    //        mOldVerScrollPos = 0;
    //    }

    //    Super::rebuildLayout();
    //    mOldHorScrollPos = 0;
    //    mOldVerScrollPos = 0;
    //    if (refreshScroll)
    //    {
    //        updateWindowScroll();
    //    }
    //}


    //void ScrollDecorator::updateHorizontalScrollInfo()
    //{
    //    Scrollbar * scrollbar = mHorizontalScrollbar->downcast<Scrollbar>();
    //    if (scrollbar)
    //    {
    //        int maxpos = Defaults::Attributes::maxpos();
    //        float ratio = (float)mDecoratedComponent->clientRect().width()/(float)mDecoratedComponent->getWidth(Minimum);
    //        int pageincrement = (int)(maxpos*ratio + 0.5);
    //        int curpos = getScrollPos(scrollbar->handle());
    //        if (ratio < 1)
    //        {
    //            Windows::setScrollInfo(scrollbar->handle(), maxpos, pageincrement, curpos);
    //            scrollbar->setHidden(mOverflowX == CSSOverflow_Hidden);
    //            scrollbar->setDisabled(false);
    //        }
    //        else
    //        {
    //            Windows::setScrollInfo(scrollbar->handle(), maxpos, pageincrement, 0);
    //            scrollbar->setHidden(mOverflowX != CSSOverflow_Scroll);
    //            scrollbar->setDisabled(true);
    //        }
    //    }
    //}


    //void ScrollDecorator::updateVerticalScrollInfo()
    //{
    //    Scrollbar * scrollbar = mVerticalScrollbar->downcast<Scrollbar>();
    //    if (scrollbar)
    //    {
    //        int maxpos = Defaults::Attributes::maxpos();
    //        float ratio = (float)mDecoratedComponent->clientRect().height()/(float)mDecoratedComponent->getHeight(Minimum);
    //        int pageincrement = (int)(maxpos*ratio + 0.5);
    //        int curpos = Windows::getScrollPos(scrollbar->handle());
    //        if (ratio < 1)
    //        {
    //            Windows::setScrollInfo(scrollbar->handle(), maxpos, pageincrement, curpos);
    //            scrollbar->setHidden(mOverflowY == CSSOverflow_Hidden);
    //            scrollbar->setDisabled(false);
    //        }
    //        else
    //        {
    //            scrollbar->setHidden(mOverflowY != CSSOverflow_Scroll);
    //            scrollbar->setDisabled(true);
    //            Windows::setScrollInfo(scrollbar->handle(), maxpos, pageincrement, 0);
    //        }
    //    }
    //}


    //void ScrollDecorator::move(int x, int y, int w, int h)
    //{
    //    // Update page height of scroll boxes
    //    int newW = w;
    //    int newH = h;
    //    if (mOverflowX != CSSOverflow_Hidden)
    //    {
    //        Scrollbar * scrollbar = mHorizontalScrollbar->downcast<Scrollbar>();
    //        if (scrollbar)
    //        {
    //            if (!scrollbar->isHidden())
    //            {
    //                newH -= Defaults::scrollbarWidth();
    //            }
    //            scrollbar->setWidth(w - Defaults::scrollbarWidth());
    //        }
    //    }

    //    if (mOverflowY != CSSOverflow_Hidden)
    //    {
    //        Scrollbar * scrollbar = mVerticalScrollbar->downcast<Scrollbar>();
    //        if (scrollbar)
    //        {
    //            if (!scrollbar->isHidden())
    //            {
    //                newW -= Defaults::scrollbarWidth();
    //            }
    //            scrollbar->setHeight(h - Defaults::scrollbarWidth());
    //        }
    //    }
    //    Super::move(x, y, newW, newH);
    //    updateHorizontalScrollInfo();
    //    updateVerticalScrollInfo();
    //}


    //void ScrollDecorator::updateWindowScroll()
    //{
    //    if (mOverflowX == CSSOverflow_Hidden && mOverflowY == CSSOverflow_Hidden)
    //    {
    //        return;
    //    }

    //    if (NativeComponent * native = mDecoratedComponent->downcast<NativeComponent>())
    //    {
    //        int maxpos = Defaults::Attributes::maxpos();
    //        Rect clientRect(mDecoratedComponent->clientRect());

    //        int newHorScrollPos = 0;
    //        int newVerScrollPos = 0;
    //        int dx = 0;
    //        int dy = 0;

    //        if (mHorizontalScrollbar)
    //        {
    //            Scrollbar * hscrollbar = mHorizontalScrollbar->downcast<Scrollbar>();
    //            int minHorSize = mDecoratedComponent->getWidth(Minimum);
    //            int horScrollPos = Windows::getScrollPos(hscrollbar->handle());
    //            double horRatio = (double)horScrollPos/(double)Defaults::Attributes::maxpos();
    //            newHorScrollPos = (int)((horRatio * (double)minHorSize) + 0.5);
    //            dx = newHorScrollPos - mOldHorScrollPos;
    //        }

    //        if (mVerticalScrollbar)
    //        {
    //            Scrollbar * vscrollbar = mVerticalScrollbar->downcast<Scrollbar>();
    //            int minVerSize = mDecoratedComponent->getHeight(Minimum);
    //            int verScrollPos = Windows::getScrollPos(vscrollbar->handle());
    //            double verRatio = (double)verScrollPos/(double)Defaults::Attributes::maxpos();
    //            newVerScrollPos = (int)((verRatio * (double)minVerSize) + 0.5);
    //            dy = newVerScrollPos - mOldVerScrollPos;
    //        }

    //        if (NativeComponent * native = mDecoratedComponent->downcast<NativeComponent>())
    //        {
    //            ::ScrollWindowEx(native->handle(), -dx, -dy, 0, 0, 0, 0, SW_SCROLLCHILDREN | SW_INVALIDATE);
    //        }
    //        mOldHorScrollPos = newHorScrollPos;
    //        mOldVerScrollPos = newVerScrollPos;
    //    }
    //}


    //LRESULT ScrollDecorator::handleMouseWheel(WPARAM wParam, LPARAM lParam)
    //{
    //    // Forward mouse wheel messages to the vertical scrollbar
    //    if (Scrollbar * vscrollbar = mVerticalScrollbar->downcast<Scrollbar>())
    //    {
    //        ::SendMessage(vscrollbar->handle(), WM_MOUSEWHEEL, wParam, lParam);
    //        return 0;
    //    }
    //    return 1;
    //}


    //bool ScrollDecorator::curposChanged(Scrollbar * inSender, int inOldPos, int inNewPos)
    //{
    //    updateWindowScroll();
    //    return true;
    //}


    MarginDecorator::MarginDecorator(Component * inDecoratedElement) :
        Decorator(inDecoratedElement),
        mTop(2),
        mLeft(4),
        mRight(4),
        mBottom(2)
    {
    }


    MarginDecorator::MarginDecorator(ComponentPtr inDecoratedElement) :
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
        if (mDecoratedComponent)
        {
            mDecoratedComponent->move(mInnerRect.x(),
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
