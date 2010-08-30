#include "XULWin/Decorators.h"
#include "XULWin/ChromeURL.h"
#include "XULWin/Conversions.h"
#include "XULWin/Defaults.h"
#include "XULWin/Element.h"
#include "XULWin/Elements.h"
#include "XULWin/Types.h"
#include "XULWin/XMLWindow.h"
#include "XULWin/WinUtils.h"
#include "XULWin/Gdiplus.h"
#include "Poco/Path.h"
#include "Poco/UnicodeConverter.h"
#include <boost/bind.hpp>
#include <boost/noncopyable.hpp>
#include <boost/scoped_ptr.hpp>


namespace XULWin
{
    using namespace WinAPI;


    MarginDecorator::MarginDecorator(Component * inDecoratedComponent) :
        Decorator(inDecoratedComponent),
        mTop(2),
        mLeft(4),
        mRight(4),
        mBottom(2)
    {
    }


    MarginDecorator::MarginDecorator(ComponentPtr inDecoratedComponent) :
        Decorator(inDecoratedComponent),
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


    ScrollDecorator::ScrollDecorator(Component * inParent,
                                     Component * inDecoratedComponent,
                                     CSSOverflow inOverflowX,
                                     CSSOverflow inOverflowY) :
        Decorator(inDecoratedComponent),
        mOverflowX(inOverflowX),
        mOverflowY(inOverflowY),
        mOldHorScrollPos(0),
        mOldVerScrollPos(0)
    {
        if (mOverflowX != CSSOverflow_Hidden)
        {
            AttributesMapping attr;
            attr["orient"] = Orient2String(Horizontal);
            mHorizontalScrollbar = XMLScrollbar::Create(inParent->el(), attr);

            // Remove it from the parent so that it is untouched by its layout manager
            inParent->el()->removeChild(mHorizontalScrollbar.get());

            mHorizontalScrollbar->component()->downcast<Scrollbar>()->setEventListener(this);
        }
        if (mOverflowY != CSSOverflow_Hidden)
        {
            AttributesMapping attr;
            attr["orient"] = Orient2String(Vertical);
            mVerticalScrollbar = XMLScrollbar::Create(inParent->el(), attr);

            // Remove it from the parent so that it is untouched by its layout manager
            inParent->el()->removeChild(mVerticalScrollbar.get());

            mVerticalScrollbar->component()->downcast<Scrollbar>()->setEventListener(this);
        }
    }


    bool ScrollDecorator::init()
    {
        Element * window = 0;
        Element * current = mDecoratedComponent->el();
        while (!window)
        {
            if (current->tagName() == XMLWindow::TagName())
            {
                window = current;
            }
            else if (current->parent())
            {
                current = current->parent();
            }
            else
            {
                break;
            }
        }
        if (window)
        {
            mEvents.connect(window, WM_MOUSEWHEEL, boost::bind(&ScrollDecorator::handleMouseWheel, this, _1, _2));
        }
        return Super::init();
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

        int result = mDecoratedComponent->getWidth(inSizeConstraint);
        if (mVerticalScrollbar && !mVerticalScrollbar->component()->isHidden())
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

        int result = mDecoratedComponent->getHeight(inSizeConstraint);
        if (mHorizontalScrollbar && !mHorizontalScrollbar->component()->isHidden())
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
            mHorizontalScrollbar->component()->move(
                clientRect.x(),
                clientRect.y() + clientRect.height(),
                clientRect.width(),
                Defaults::scrollbarWidth());
            mOldHorScrollPos = 0;
        }
        if (mOverflowY != CSSOverflow_Hidden)
        {
            mVerticalScrollbar->component()->move(
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


    void ScrollDecorator::updateHorizontalScrollInfo()
    {
        if (!mHorizontalScrollbar)
        {
            return;
        }

        Scrollbar * scrollbar = mHorizontalScrollbar->component()->downcast<Scrollbar>();
        if (scrollbar)
        {
            int maxpos = Defaults::Attributes::maxpos();
            float a = (float)mDecoratedComponent->clientRect().width();
            float b = (float)mDecoratedComponent->getWidth();
            if (b == 0)
            {
                b = 1;
            }

            float ratio = a/b;
            int pageincrement = (int)(maxpos*ratio + 0.5);
            int curpos = Scrollbar_GetPos(scrollbar->handle());
            if (ratio < 1)
            {
                WinAPI::Scrollbar_SetInfo(scrollbar->handle(), maxpos, pageincrement, curpos);
                scrollbar->setHidden(mOverflowX == CSSOverflow_Hidden);
                scrollbar->setDisabled(false);
            }
            else
            {
                WinAPI::Scrollbar_SetInfo(scrollbar->handle(), maxpos, pageincrement, 0);
                scrollbar->setHidden(mOverflowX != CSSOverflow_Scroll);
                scrollbar->setDisabled(true);
            }
        }
    }


    void ScrollDecorator::updateVerticalScrollInfo()
    {
        if (!mVerticalScrollbar)
        {
            return;
        }

        Scrollbar * scrollbar = mVerticalScrollbar->component()->downcast<Scrollbar>();
        if (scrollbar)
        {
            int maxpos = Defaults::Attributes::maxpos();
            float a = (float)mDecoratedComponent->clientRect().height();
            float b = (float)mDecoratedComponent->getHeight();
            if (b == 0)
            {
                b = 1;
            }

            float ratio = a/b;
            int pageincrement = (int)(maxpos*ratio + 0.5);
            int curpos = WinAPI::Scrollbar_GetPos(scrollbar->handle());
            if (ratio < 1)
            {
                WinAPI::Scrollbar_SetInfo(scrollbar->handle(), maxpos, pageincrement, curpos);
                scrollbar->setHidden(mOverflowY == CSSOverflow_Hidden);
                scrollbar->setDisabled(false);
            }
            else
            {
                scrollbar->setHidden(mOverflowY != CSSOverflow_Scroll);
                scrollbar->setDisabled(true);
                WinAPI::Scrollbar_SetInfo(scrollbar->handle(), maxpos, pageincrement, 0);
            }
        }
    }


    void ScrollDecorator::move(int x, int y, int w, int h)
    {
        // Update page height of scroll boxes
        int newW = w;
        int newH = h;
        if (mOverflowX != CSSOverflow_Hidden)
        {
            Scrollbar * scrollbar = mHorizontalScrollbar->component()->downcast<Scrollbar>();
            if (scrollbar)
            {
                if (!scrollbar->isHidden())
                {
                    newH -= Defaults::scrollbarWidth();
                }
                scrollbar->setWidth(w - Defaults::scrollbarWidth());
            }
        }

        if (mOverflowY != CSSOverflow_Hidden)
        {
            Scrollbar * scrollbar = mVerticalScrollbar->component()->downcast<Scrollbar>();
            if (scrollbar)
            {
                if (!scrollbar->isHidden())
                {
                    newW -= Defaults::scrollbarWidth();
                }
                scrollbar->setHeight(h - Defaults::scrollbarWidth());
            }
        }
        Super::move(x, y, newW, newH);
        updateHorizontalScrollInfo();
        updateVerticalScrollInfo();
    }


    void ScrollDecorator::updateWindowScroll()
    {
        if (mOverflowX == CSSOverflow_Hidden && mOverflowY == CSSOverflow_Hidden)
        {
            return;
        }

        if (NativeComponent * native = mDecoratedComponent->downcast<NativeComponent>())
        {
            Rect clientRect(mDecoratedComponent->clientRect());

            int newHorScrollPos = 0;
            int newVerScrollPos = 0;
            int dx = 0;
            int dy = 0;

            if (mHorizontalScrollbar)
            {
                Scrollbar * hscrollbar = mHorizontalScrollbar->component()->downcast<Scrollbar>();
                int minHorSize = mDecoratedComponent->getWidth(Minimum);
                int horScrollPos = WinAPI::Scrollbar_GetPos(hscrollbar->handle());
                double horRatio = (double)horScrollPos/(double)Defaults::Attributes::maxpos();
                newHorScrollPos = (int)((horRatio * (double)minHorSize) + 0.5);
                dx = newHorScrollPos - mOldHorScrollPos;
            }

            if (mVerticalScrollbar)
            {
                Scrollbar * vscrollbar = mVerticalScrollbar->component()->downcast<Scrollbar>();
                int minVerSize = mDecoratedComponent->getHeight(Minimum);
                int verScrollPos = WinAPI::Scrollbar_GetPos(vscrollbar->handle());
                double verRatio = (double)verScrollPos/(double)Defaults::Attributes::maxpos();
                newVerScrollPos = (int)((verRatio * (double)minVerSize) + 0.5);
                dy = newVerScrollPos - mOldVerScrollPos;
            }

            if (NativeComponent * native = mDecoratedComponent->downcast<NativeComponent>())
            {
                ::ScrollWindowEx(native->handle(), -dx, -dy, 0, 0, 0, 0, SW_SCROLLCHILDREN | SW_INVALIDATE);
            }
            mOldHorScrollPos = newHorScrollPos;
            mOldVerScrollPos = newVerScrollPos;
        }
    }


    LRESULT ScrollDecorator::handleMouseWheel(WPARAM wParam, LPARAM lParam)
    {
        // Forward mouse wheel messages to the vertical scrollbar
        if (Scrollbar * vscrollbar = mVerticalScrollbar->component()->downcast<Scrollbar>())
        {
            ::SendMessage(vscrollbar->handle(), WM_MOUSEWHEEL, wParam, lParam);
            return 0;
        }
        return 1;
    }


    bool ScrollDecorator::curposChanged(Scrollbar * inSender, int inOldPos, int inNewPos)
    {
        updateWindowScroll();
        return true;
    }

} // namespace XULWin
