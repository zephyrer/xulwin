#include "XULWin/ToolbarItem.h"
#include "XULWin/Toolbar.h"
#include "XULWin/PopupMenu.h"
#include "XULWin/Unicode.h"
#include "XULWin/WinUtils.h"
#include <boost/bind.hpp>
#include <gdiplus.h>
#include <commctrl.h>

#ifdef _DEBUGFREE // detect the access to freed memory
#undef free
#define free(p) _free_dbg(p, _NORMAL_BLOCK); *(int*)&p = 0x666;
#endif


namespace XULWin
{

    namespace Windows
    {

        extern const int cDownArrowWidth;
        extern const int cMarginForCustomWindow;
        extern const int cSpacingBetweenIconAndText;

        static boost::shared_ptr<Gdiplus::Bitmap> nullImage;

        RECT getTextRect(const ConcreteToolbarItem * inItem, const RECT & inRect, size_t inIconWidth, size_t inIconHeight, SIZE inTextSize)
        {
            RECT result;
            result.left = inRect.left + inItem->getLeftMargin();;
            if (inIconWidth != 0)
            {
                result.left += inIconWidth + cSpacingBetweenIconAndText;
            }
            result.top = inRect.top + ((inRect.bottom - inRect.top) - inTextSize.cy)/2;
            result.right = inRect.right - inItem->getRightMargin();;
            result.bottom = result.top + inTextSize.cy;
            return result;
        }


        ConcreteToolbarItem::ConcreteToolbarItem
        (
            boost::weak_ptr<Toolbar> inToolbar,
            int inCommandID,
            const std::string & inText,
            const std::string & inTooltipText,
            boost::shared_ptr<Gdiplus::Bitmap> inImage
        ):
            mToolbar(inToolbar),
            mCommandId(inCommandID),
            mText(inText),
            mTooltipText(inTooltipText),
            mImage(inImage),
            mNoHover(false),
            mLeftMargin(4),
            mRightMargin(4),
            mMinimumWidth(0),
            mMaximumWidth(INT_MAX),
            mMaxIconHeight(512)
        {
        }


        ConcreteToolbarItem::~ConcreteToolbarItem()
        {
        }


        bool ConcreteToolbarItem::noHover() const
        {
            return mNoHover;
        }


        int ConcreteToolbarItem::getLeftMargin() const
        {
            return mLeftMargin;
        }


        void ConcreteToolbarItem::setLeftMargin(int inLeftMargin)
        {
            mLeftMargin = inLeftMargin;
        }


        void ConcreteToolbarItem::setRightMargin(int inRightMargin)
        {
            mRightMargin = inRightMargin;
        }


        int ConcreteToolbarItem::getRightMargin() const
        {
            return mRightMargin;
        }


        int ConcreteToolbarItem::minimumWidth() const
        {
            return mMinimumWidth;
        }


        void ConcreteToolbarItem::setMinimumWidth(int inMinimumWidth)
        {
            mMinimumWidth = inMinimumWidth;
        }


        int ConcreteToolbarItem::maximumWidth() const
        {
            return mMaximumWidth;
        }


        void ConcreteToolbarItem::setMaximumWidth(int inMaximumWidth)
        {
            mMaximumWidth = inMaximumWidth;
        }


        int ConcreteToolbarItem::maxIconHeight() const
        {
            return mMaxIconHeight;
        }


        void ConcreteToolbarItem::setMaxIconHeight(int inMaxIconHeight)
        {
            mMaxIconHeight = inMaxIconHeight;
        }


        void ConcreteToolbarItem::setNoHover(bool inNoHover)
        {
            mNoHover = inNoHover;
        }


        bool ConcreteToolbarItem::getIndex(size_t & outIndex) const
        {
            if (boost::shared_ptr<Toolbar> ieToolbar = mToolbar.lock())
            {
                for (size_t idx = 0; idx != ieToolbar->size(); ++idx)
                {
                    if (ieToolbar->get(idx)->commandId() == commandId())
                    {
                        outIndex = idx;
                        return true;
                    }
                }
            }
            return false;
        }


        RECT ConcreteToolbarItem::getRect() const
        {
            RECT result = {0, 0, 0, 0};
            if (boost::shared_ptr<Toolbar> ieToolbar = mToolbar.lock())
            {
                size_t index;
                if (getIndex(index))
                {
                    SendMessage(ieToolbar->handle(), TB_GETITEMRECT, (WPARAM)(INT)index, (LPARAM)&result);
                }
            }
            return result;
        }


        void ConcreteToolbarItem::setVisible(bool inVisible)
        {
            if (boost::shared_ptr<Toolbar> ieToolbar = mToolbar.lock())
            {
                SendMessage(ieToolbar->handle(), TB_HIDEBUTTON, (WPARAM)(INT)commandId(), (LPARAM)MAKELONG(inVisible ? FALSE : TRUE, 0));
            }
        }


        bool ConcreteToolbarItem::isVisible() const
        {
            if (boost::shared_ptr<Toolbar> ieToolbar = mToolbar.lock())
            {
                return 0 == SendMessage(ieToolbar->handle(), TB_ISBUTTONHIDDEN, (WPARAM)(INT)commandId(), (LPARAM)0);
            }
            return false;
        }


        int ConcreteToolbarItem::commandId() const
        {
            return mCommandId;
        }


        const std::string & ConcreteToolbarItem::text() const
        {
            return mText;
        }


        const std::string & ConcreteToolbarItem::tooltipText() const
        {
            return mTooltipText;
        }


        void ConcreteToolbarItem::setText(const std::string & inText)
        {
            mText = inText;
            boost::shared_ptr<Toolbar> toolbar = mToolbar.lock();
            if (toolbar)
            {
                toolbar->rebuildLayout();

                // HACK!
                // This code forces the layout to update but does not belong here.
                RECT rc;
                GetClientRect(toolbar->handle(), &rc);
                for (size_t idx = 0; idx != toolbar->size(); ++idx)
                {
                    toolbar->get(idx)->onClientRectChanged(rc);
                }
                // END HACK
            }
        }


        boost::shared_ptr<Gdiplus::Bitmap> ConcreteToolbarItem::image() const
        {
            return mImage;
        }


        void ConcreteToolbarItem::setImage(boost::shared_ptr<Gdiplus::Bitmap> inImage)
        {
            mImage = inImage;
            if (boost::shared_ptr<Toolbar> toolbar = mToolbar.lock())
            {
                toolbar->rebuildLayout();
            }
        }


        void ConcreteToolbarItem::draw(HDC inHDC, RECT rect, HFONT hFont, SIZE inTextSize)
        {
            size_t imageWidth = 0;
            size_t imageHeight = 0;
            if (mImage)
            {
                double resizeFactor = static_cast<double>(std::min<size_t>(maxIconHeight(), mImage->GetHeight()))/static_cast<double>(mImage->GetHeight());
                int h = static_cast<size_t>(static_cast<double>(mImage->GetHeight() * resizeFactor) + 0.5);
                int w = static_cast<size_t>(static_cast<double>(mImage->GetWidth() * resizeFactor) + 0.5);
                int x = rect.left + mLeftMargin;
                int y = rect.top + ((rect.bottom - rect.top) - h)/2;
                Gdiplus::Graphics g(inHDC);
                g.SetInterpolationMode(Gdiplus::InterpolationModeHighQuality);
                g.SetSmoothingMode(Gdiplus::SmoothingModeHighQuality);
                g.DrawImage
                (
                    mImage.get(),
                    x,
                    y,
                    w,
                    h
                );
                imageWidth = w;
                imageHeight = h;
            }

            // draw text
            std::wstring unicodeText(ToUTF16(mText));
            RECT textRect = getTextRect(this, rect, imageWidth, imageHeight, inTextSize);
            int oldBkMode = ::SetBkMode(inHDC, TRANSPARENT);
            DWORD fgColor = GetSysColor(COLOR_MENUTEXT);
            int oldTextColor = ::SetTextColor(inHDC, fgColor);
            HGDIOBJ oldFont = SelectObject(inHDC, hFont);
            ::DrawText(inHDC, unicodeText.c_str(), unicodeText.length(), &textRect, DT_LEFT | DT_VCENTER | DT_SINGLELINE);

            // Release stuff
            ::SetTextColor(inHDC, oldTextColor);
            ::SetBkMode(inHDC, oldBkMode);
            ::SelectObject(inHDC, oldFont);
        }


        ToolbarButtonElement::ToolbarButtonElement
        (
            boost::weak_ptr<Toolbar> inToolbar,
            int inCommandID,
            const boost::function<void()> & inAction,
            const std::string & inText,
            const std::string & inTooltipText,
            boost::shared_ptr<Gdiplus::Bitmap> inImage
        ):
            ConcreteToolbarItem
            (
                inToolbar,
                inCommandID,
                inText,
                inTooltipText,
                inImage
            ),
            mAction(inAction)
        {
        }


        ToolbarButtonElement::~ToolbarButtonElement()
        {
        }


        int ToolbarButtonElement::flags() const
        {
            return BTNS_BUTTON | (text().empty() ? 0 : BTNS_SHOWTEXT);
        }


        void ToolbarButtonElement::performCommand()
        {
            if (mAction)
            {
                mAction();
            }
        }


        ToolbarDropDown::ToolbarDropDown
        (
            boost::weak_ptr<Toolbar> inToolbar,
            ToolbarDropDown::EventHandler * inEventHandler,
            int inCommandID,
            const std::string & inText,
            const std::string & inTooltipText,
            boost::shared_ptr<Gdiplus::Bitmap> inImage,
            bool inIsButton
        ):
            ConcreteToolbarItem
            (
                inToolbar,
                inCommandID,
                inText,
                inTooltipText,
                inImage
            ),
            mEventHandler(inEventHandler),
            mIsButton(inIsButton)
        {
            if (inIsButton)
            {
                setRightMargin(getRightMargin() + 8);
            }
        }


        ToolbarDropDown::~ToolbarDropDown()
        {
        }


        int ToolbarDropDown::flags() const
        {
            return BTNS_BUTTON | (isButton() ? BTNS_DROPDOWN : BTNS_WHOLEDROPDOWN) | (text().empty() ? 0 : BTNS_SHOWTEXT);
        }


        bool ToolbarDropDown::isButton() const
        {
            return mIsButton;
        }


        void ToolbarDropDown::showToolbarMenu(RECT inToolbarButtonRect)
        {
            if (mEventHandler)
            {
                mEventHandler->showToolbarMenu(inToolbarButtonRect);
            }
        }


        ToolbarSeparator::ToolbarSeparator
        (
            boost::weak_ptr<Toolbar> inToolbar,
            int inCommandID
        ):
            ConcreteToolbarItem
            (
                inToolbar,
                inCommandID,
                "",
                "",
                nullImage
            )
        {
            setNoHover(true);
        }


        ToolbarSeparator::~ToolbarSeparator()
        {
        }


        int ToolbarSeparator::flags() const
        {
            return BTNS_BUTTON;
        }


        void ToolbarSeparator::draw(HDC inHDC, RECT inRect, HFONT hFont, SIZE inTextSize)
        {
            Gdiplus::Graphics g(inHDC);
            Gdiplus::Pen pen(Gdiplus::Color::DarkGray, 1.0);
            const static int cMarginTopBottom = 2;
            int x1 = inRect.left + (inRect.right-inRect.left)/2;
            int x2 = x1;
            int y1 = inRect.top + cMarginTopBottom;
            int y2 = inRect.bottom - 2*cMarginTopBottom;
            g.DrawLine(&pen, x1, y1, x2, y2);
        }


        ToolbarSpring::ToolbarSpring
        (
            boost::weak_ptr<Toolbar> inToolbar,
            int inCommandID
        ):
            ConcreteToolbarItem
            (
                inToolbar,
                inCommandID,
                "",
                "",
                nullImage
            )
        {
            setNoHover(true);
        }


        int ToolbarSpring::flags() const
        {
            return BTNS_SEP;
        }


        IECustomWindow::IECustomWindow()
        {
        }


        IECustomWindow::~IECustomWindow()
        {
        }


        int IECustomWindow::flags() const
        {
            return BTNS_SEP;
        }


    } // namespace Windows

} // namespace XULWin
