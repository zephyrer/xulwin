#include "XULWin/Scrollbar.h"
#include "XULWin/Decorator.h"
#include "XULWin/Conversions.h"
#include "XULWin/Defaults.h"
#include "XULWin/ErrorReporter.h"
#include "XULWin/WinUtils.h"


namespace XULWin
{
    
    Component * CreateScrollbar(Component * inComponent, const AttributesMapping & inAttr)
    {
        return new Decorator(new Scrollbar(inComponent, inAttr));
    }


    DWORD Scrollbar::GetFlags(const AttributesMapping & inAttributesMapping)
    {
        DWORD flags = 0;
        AttributesMapping::const_iterator it = inAttributesMapping.find("orient");
        if (it != inAttributesMapping.end())
        {
            const std::string & orient = it->second;
            if (orient == "horizontal")
            {
                flags |= SBS_HORZ | SBS_RIGHTALIGN;
            }
            else if (orient == "vertical")
            {
                flags |= SBS_VERT | SBS_BOTTOMALIGN;
            }
            else
            {
                ReportError("Invalid orient found for scrollbar!");
            }
        }
        return flags;
    }


    Scrollbar::Scrollbar(Component * inParent, const AttributesMapping & inAttributesMapping) :
        NativeControl(inParent, inAttributesMapping,
                      TEXT("SCROLLBAR"),
                      0, // exStyle
                      WS_TABSTOP | GetFlags(inAttributesMapping)),
        mEventListener(0),
        mIncrement(0)
    {
        mExpansive = true;
        WinAPI::setScrollInfo(handle(), 100, 10, 0);
    }


    int Scrollbar::calculateWidth(SizeConstraint inSizeConstraint) const
    {
        return Defaults::scrollbarWidth();
    }


    int Scrollbar::calculateHeight(SizeConstraint inSizeConstraint) const
    {
        return Defaults::scrollbarWidth();
    }


    LRESULT Scrollbar::handleMessage(UINT inMessage, WPARAM wParam, LPARAM lParam)
    {
        if (WM_VSCROLL == inMessage || WM_HSCROLL == inMessage)
        {
            int totalHeight = 0;
            int pageHeight = 0;
            int currentPosition = 0;
            WinAPI::getScrollInfo(handle(), totalHeight, pageHeight, currentPosition);
            switch (LOWORD(wParam))
            {
                case SB_LINEUP: // user clicked the top arrow
                {
                    currentPosition -= 1;
                    break;
                }
                case SB_LINEDOWN: // user clicked the bottom arrow
                {
                    currentPosition += 1;
                    break;
                }
                case SB_PAGEUP: // user clicked the scroll bar shaft above the scroll box
                {
                    currentPosition -= pageHeight;
                    break;
                }
                case SB_PAGEDOWN: // user clicked the scroll bar shaft below the scroll box
                {
                    currentPosition += pageHeight;
                    break;
                }
                case SB_THUMBTRACK: // user dragged the scroll box
                {
                    currentPosition = HIWORD(wParam);
                    break;
                }
                default:
                {
                    break;
                }
            }
            if (currentPosition < 0)
            {
                currentPosition = 0;
            }
            if (currentPosition > totalHeight)
            {
                currentPosition = totalHeight;
            }

            setAttribute("curpos", Int2String(currentPosition));
            return 0;
        }
        else if (WM_MOUSEWHEEL == inMessage)
        {
            short numDelta = HIWORD(wParam);
            short numPages = numDelta / WHEEL_DELTA;
            int totalHeight = 0;
            int pageHeight = 0;
            int currentPosition = 0;
            WinAPI::getScrollInfo(handle(), totalHeight, pageHeight, currentPosition);
            currentPosition = currentPosition - numPages*pageHeight;
            if (currentPosition < 0)
            {
                currentPosition = 0;
            }
            if (currentPosition > totalHeight)
            {
                currentPosition = totalHeight;
            }
            setAttribute("curpos", Int2String(currentPosition));
            return 0;
        }
        return NativeControl::handleMessage(inMessage, wParam, lParam);
    }


    int Scrollbar::getCurrentPosition() const
    {
        return WinAPI::getScrollPos(handle());
    }


    void Scrollbar::setCurrentPosition(int inCurrentPosition)
    {
        int totalHeight = 0;
        int pageHeight = 0;
        int oldCurPos = 0;
        WinAPI::getScrollInfo(handle(), totalHeight, pageHeight, oldCurPos);

        // The order in which curpos, maxpos and pageincrement
        // will be set (alphabetically by attribute name) can cause
        // impossible scrollbar states (e.g. currentpos or pageincrement
        // greater than maxpos). And we want to avoid that.
        // Our workaround is to detect such states here, and change invalid
        // values to valid ones.
        if (pageHeight == 0)
        {
            pageHeight = 1;
        }
        if (totalHeight < pageHeight)
        {
            totalHeight = pageHeight + 1;
        }
        if (totalHeight < inCurrentPosition)
        {
            totalHeight = inCurrentPosition + 1;
        }
        WinAPI::setScrollInfo(handle(), totalHeight, pageHeight, inCurrentPosition);
        if ((oldCurPos != inCurrentPosition) && eventHandler())
        {
            eventHandler()->curposChanged(this, oldCurPos, inCurrentPosition);
        }
    }


    int Scrollbar::getMaxPosition() const
    {
        int totalHeight = 0;
        int pageHeight = 0;
        int curPos = 0;
        WinAPI::getScrollInfo(handle(), totalHeight, pageHeight, curPos);
        return totalHeight;
    }


    void Scrollbar::setMaxPosition(int inMaxPosition)
    {
        int dummy = 0;
        int pageHeight = 0;
        int curPos = 0;
        WinAPI::getScrollInfo(handle(), dummy, pageHeight, curPos);

        // The order in which setCurPos, setMaxPos and setPageIncrement
        // will be set (alphabetically by attribute name) can cause
        // impossible scrollbar states (e.g. currentpos or pageincrement
        // greater than maxpos). And we want to avoid that.
        // Our workaround is to detect such states here, and change invalid
        // values to valid ones.
        if (pageHeight == 0)
        {
            pageHeight = 1;
        }
        if (inMaxPosition <= pageHeight)
        {
            pageHeight = inMaxPosition - 1;
        }
        WinAPI::setScrollInfo(handle(), inMaxPosition, pageHeight, curPos);
    }


    void Scrollbar::setIncrement(int inIncrement)
    {
        mIncrement = inIncrement;
    }


    int Scrollbar::getIncrement() const
    {
        return mIncrement;
    }


    void Scrollbar::setPageIncrement(int inPageIncrement)
    {
        int totalHeight = 0;
        int dummy = 0;
        int curPos = 0;
        WinAPI::getScrollInfo(handle(), totalHeight, dummy, curPos);

        // The order in which setCurPos, setMaxPos and setPageIncrement
        // will be set (alphabetically by attribute name) can cause
        // impossible scrollbar states (e.g. currentpos or pageincrement
        // greater than maxpos). And we want to avoid that.
        // Our workaround is to detect such states here, and change invalid
        // values to valid ones.
        if (totalHeight == 0)
        {
            totalHeight = 1;
        }
        if (curPos > totalHeight)
        {
            totalHeight = curPos + 1;
        }
        if (inPageIncrement >= totalHeight)
        {
            totalHeight = inPageIncrement + 1;
        }
        WinAPI::setScrollInfo(handle(), totalHeight, inPageIncrement, curPos);
    }


    int Scrollbar::getPageIncrement() const
    {
        int totalHeight = 0;
        int pageHeight = 0;
        int curPos = 0;
        WinAPI::getScrollInfo(handle(), totalHeight, pageHeight, curPos);
        return pageHeight;
    }


    bool Scrollbar::initAttributeControllers()
    {
        setAttributeController<ScrollbarCurrentPositionController>(this);
        setAttributeController<ScrollbarMaxPositionController>(this);
        setAttributeController<ScrollbarIncrementController>(this);
        setAttributeController<ScrollbarPageIncrementController>(this);
        return Super::initAttributeControllers();
    }

} // namespace XULWin
