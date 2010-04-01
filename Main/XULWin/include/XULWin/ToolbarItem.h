#ifndef TOOLBARITEM_H_INCLUDED
#define TOOLBARITEM_H_INCLUDED


#include "XULWin/PopupMenu.h" 
#include "XULWin/Windows.h"
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/utility.hpp>


namespace Gdiplus
{
    class Bitmap;
}


namespace XULWin
{

namespace Windows
{

    class Toolbar;
    class ConcreteToolbarItem;

    class AbstractToolbarItem : public boost::noncopyable
    {
    public:            
        friend class Toolbar;
        
        virtual ~AbstractToolbarItem() = 0{}
        
        virtual int flags() const = 0;

        virtual void performCommand() = 0;

        virtual int commandId() const = 0;

    protected:
        friend class Toolbar;
        friend class ConcreteToolbarItem;

        virtual void onPostRebuildLayout(){}

        virtual void onClientRectChanged(const RECT & inNewClientRect){}
    };


    class ConcreteToolbarItem : public AbstractToolbarItem
    {
    public:            
        friend class Toolbar;
        ConcreteToolbarItem
        (
            boost::weak_ptr<Toolbar> inToolbar,
            int inCommandID,
            const std::string & inText,
            const std::string & inTooltipText,
            boost::shared_ptr<Gdiplus::Bitmap> inImage
        );
        
        virtual ~ConcreteToolbarItem();
        
        virtual int flags() const = 0;

        virtual void performCommand() = 0;
        
        virtual int commandId() const;

        bool noHover() const;

        void setNoHover(bool inNoHover);
        
        const std::string & text() const;

        const std::string & tooltipText() const;

        boost::shared_ptr<Gdiplus::Bitmap> image() const;

        void setText( const std::string & inText);

        void setImage(boost::shared_ptr<Gdiplus::Bitmap> inImage);

        int getLeftMargin() const;

        int getRightMargin() const;

        void setLeftMargin(int inLeftMargin);

        void setRightMargin(int inRightMargin);

        int minimumWidth() const;

        void setMinimumWidth(int inMinimumWidth);

        int maximumWidth() const;

        void setMaximumWidth(int inMaximumWidth);

        int maxIconHeight() const;

        /**
         * Use this method to auto-shrink icons that are too large.
         */
        void setMaxIconHeight(int inMaxIconHeight);

        // gets the position of this button in its parent toolbar
        // returns false if not found
        bool getIndex(size_t & outIndex) const;

        RECT getRect() const;

        bool isVisible() const;

        void setVisible(bool inVisible);

    protected:
        virtual void draw(HDC inHDC, RECT inRect, HFONT hFont, SIZE inTextSize);

        virtual void onClientRectChanged(const RECT & inNewClientRect) {}

        virtual void onPostRebuildLayout() {}

        boost::weak_ptr<Toolbar> mToolbar;
        
    private:
        int mCommandId;
        std::string mText;
        std::string mTooltipText;
        boost::shared_ptr<Gdiplus::Bitmap> mImage;
        bool mNoHover;
        int mLeftMargin;
        int mRightMargin;
        int mMinimumWidth;
        int mMaximumWidth;
        int mMaxIconHeight;
    };
    
    class ToolbarButtonElement : public ConcreteToolbarItem
    {
    public:
        ToolbarButtonElement
        (
            boost::weak_ptr<Toolbar> inToolbar,
            int inCommandID,
            const boost::function<void()> & inAction,
            const std::string & inText,
            const std::string & inTooltipText,
            boost::shared_ptr<Gdiplus::Bitmap> inImage
        );

        virtual ~ToolbarButtonElement();

        virtual int flags() const;

        virtual void performCommand();
    private:
        boost::function<void()> mAction;
    };
    
    class ToolbarDropDown : public ConcreteToolbarItem
    {
    public:
        class EventHandler
        {
        public:
            virtual void showToolbarMenu(RECT inToolbarButtonRect) = 0;
        };

        ToolbarDropDown
        (
            boost::weak_ptr<Toolbar> inToolbar,
            EventHandler * inEventHandler,
            int inCommandID,
            const std::string & inText,
            const std::string & inTooltipText,
            boost::shared_ptr<Gdiplus::Bitmap> inImage,
            bool inIsButton
        );

        virtual ~ToolbarDropDown();

        virtual int flags() const;

        bool isButton() const;

        // forwards call to event handler
        // Rect must have screen coordinates
        void showToolbarMenu(RECT inToolbarButtonRect);

        virtual void performCommand() {}

        virtual void performMenuCommand(int inMenuId){}
        
    private:
        bool mIsButton;
        EventHandler * mEventHandler;
    };
    
    // Only one spring can be added to the toolbar.
    class ToolbarSpring : public ConcreteToolbarItem
    {
    public:
        ToolbarSpring
        (
            boost::weak_ptr<Toolbar> inToolbar,
            int inCommandID // only serves as id, no command can be associated with it
        );

        virtual ~ToolbarSpring() {}

        virtual int flags() const;

        virtual void performCommand() {}
    };

    /**
     * Interface for toolbar items that can
     * have focus.
     */
    class IFocusableToolbarItem
    {
    public:
        virtual ~IFocusableToolbarItem() {}

        virtual bool hasFocus() const = 0;

        virtual void setFocus() const = 0;
    };

    
    class IECustomWindow : public AbstractToolbarItem,
                           public IFocusableToolbarItem
    {
    public:
        IECustomWindow();

        virtual ~IECustomWindow();

        virtual int flags() const;

        virtual bool hasFocus() const = 0;

        virtual void setFocus() const = 0;

        virtual HWND handle() const = 0;

        virtual int commandId() const = 0;

        virtual void performCommand() {}
    };

    
    class ToolbarSeparator : public ConcreteToolbarItem //IECustomWindow
    {
    public:
        ToolbarSeparator
        (
            boost::weak_ptr<Toolbar> inToolbar,
            int inCommandID // only serves as id, no command can be associated with it
        );

        virtual ~ToolbarSeparator();

        virtual int flags() const;

        virtual void performCommand() {}

    private:
        virtual void draw(HDC inHDC, RECT inRect, HFONT hFont, SIZE inTextSize);
    };

} // namespace Windows

} // namespace XULWin


#endif // TOOLBARITEM_H_INCLUDED
