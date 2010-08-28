#ifndef COMPONENTS_H_INCLUDED
#define COMPONENTS_H_INCLUDED


#include "XULWin/BoxLayouter.h"
#include "XULWin/EventListener.h"
#include "XULWin/PhonyComponent.h"
#include "XULWin/NativeControl.h"
#include "XULWin/Node.h"
#include <boost/function.hpp>
#include <boost/noncopyable.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <map>
#include <string>


namespace XULWin
{

    /**
     * Label
     *
     * Native implementation for a XUL label.
     */
    class Label : public NativeControl,
                  public virtual StringValueController,
                  public virtual CSSTextAlignController
    {
    public:
        typedef NativeControl Super;

        Label(Component * inParent, const AttributesMapping & inAttr);

        // StringValueController methods
        virtual std::string getValue() const;

        virtual void setValue(const std::string & inStringValue);

        // CSSTextAlignController methods
        virtual CSSTextAlign getCSSTextAlign() const;

        virtual void setCSSTextAlign(CSSTextAlign inValue);

        virtual bool initAttributeControllers();

        virtual bool initStyleControllers();

        virtual int calculateWidth(SizeConstraint inSizeConstraint) const;

        virtual int calculateHeight(SizeConstraint inSizeConstraint) const;
    };

    /**
     * Button
     *
     * Native implementation for a XUL button.
     */
    class Button : public NativeControl
    {
    public:
        typedef NativeControl Super;

        Button(Component * inParent, const AttributesMapping & inAttr);

        virtual int calculateWidth(SizeConstraint inSizeConstraint) const;

        virtual int calculateHeight(SizeConstraint inSizeConstraint) const;
    };


    /**
     * Checkbox
     *
     * Native implementation for a XUL checkbox.
     */
    class CheckBox : public NativeControl,
                     public virtual CheckedController
    {
    public:
        typedef NativeControl Super;

        CheckBox(Component * inParent, const AttributesMapping & inAttr);

        // CheckedController methods
        virtual bool isChecked() const;

        virtual void setChecked(bool inChecked);

        virtual bool initAttributeControllers();

        virtual int calculateWidth(SizeConstraint inSizeConstraint) const;

        virtual int calculateHeight(SizeConstraint inSizeConstraint) const;
    };


    /**
    * Description
    *
    * Native implementation for a XUL description element.
    */
    class Description : public NativeControl,
                        public virtual StringValueController
    {
    public:
        typedef NativeControl Super;

        Description(Component * inParent, const AttributesMapping & inAttr);

        // StringValueController methods
        virtual std::string getValue() const;

        virtual void setValue(const std::string & inStringValue);

        virtual bool initAttributeControllers();

        virtual int calculateWidth(SizeConstraint inSizeConstraint) const;

        virtual int calculateHeight(SizeConstraint inSizeConstraint) const;
    };


    class ScopedEventListener;


    /**
     * Hyperlink
     *
     * Native implementation for a HTML hyperlink.
     */
    class Hyperlink : public Label,
                      public virtual HrefController
    {
    public:
        typedef Label Super;

        Hyperlink(Component * inParent, const AttributesMapping & inAttr);

        virtual bool init();

        // StringValueController methods
        virtual std::string getHref() const;

        virtual void setHref(const std::string & inStringValue);

        virtual bool initAttributeControllers();

    private:
        LRESULT onLButtonDown(WPARAM wParam, LPARAM lParam);
        ScopedEventListener mEvents;
        std::string mHref;
    };


    class TextBox : public NativeControl,
                    public virtual StringValueController,
                    public virtual ReadOnlyController,
                    public virtual RowsController
    {
    public:
        typedef NativeControl Super;

        TextBox(Component * inParent, const AttributesMapping & inAttr);

        // StringValueController methods
        virtual std::string getValue() const;

        virtual void setValue(const std::string & inStringValue);

        // ReadOnlyController methods
        virtual bool isReadOnly() const;

        virtual void setReadOnly(bool inReadOnly);

        // RowsControll methods
        virtual int getRows() const;

        virtual void setRows(int inRows);

        virtual bool initAttributeControllers();

        virtual int calculateWidth(SizeConstraint inSizeConstraint) const;

        virtual int calculateHeight(SizeConstraint inSizeConstraint) const;

        virtual LRESULT handleMessage(UINT inMessage, WPARAM wParam, LPARAM lParam);

    private:
        int mRows;
        static DWORD GetFlags(const AttributesMapping & inAttr);
        static DWORD GetStyleFlags(const AttributesMapping & inAttr);
    };



    class Scrollbar : public NativeControl,
                      public virtual ScrollbarCurrentPositionController,
                      public virtual ScrollbarMaxPositionController,
                      public virtual ScrollbarIncrementController,
                      public virtual ScrollbarPageIncrementController
    {
    public:
        typedef NativeControl Super;

        Scrollbar(Component * inParent, const AttributesMapping & inAttributesMapping);

        virtual int getCurrentPosition() const;

        virtual void setCurrentPosition(int inCurrentPosition);

        virtual int getMaxPosition() const;

        virtual void setMaxPosition(int inMaxPosition);

        virtual int getIncrement() const;

        virtual void setIncrement(int inIncrement);

        virtual int getPageIncrement() const;

        virtual void setPageIncrement(int inPageIncrement);

        class EventListener
        {
        public:
            virtual bool curposChanged(Scrollbar * inSender, int inOldPos, int inNewPos) = 0;
        };

        EventListener * eventHandler()
        {
            return mEventListener;
        }

        void setEventListener(EventListener * inEventListener)
        {
            mEventListener = inEventListener;
        }

        virtual int calculateWidth(SizeConstraint inSizeConstraint) const;

        virtual int calculateHeight(SizeConstraint inSizeConstraint) const;

        bool initAttributeControllers();

        virtual LRESULT handleMessage(UINT inMessage, WPARAM wParam, LPARAM lParam);

    private:
        static DWORD GetFlags(const AttributesMapping & inAttributesMapping);

        EventListener * mEventListener;
        int mIncrement;
    };


    class VirtualBox : public VirtualComponent,
                       public BoxLayouter::ContentProvider
    {
    public:
        typedef VirtualComponent Super;

        VirtualBox(Component * inParent, const AttributesMapping & inAttr);

        virtual Orient getOrient() const;

        virtual Align getAlign() const;

        virtual bool initAttributeControllers();

        virtual void rebuildLayout();

        virtual int calculateWidth(SizeConstraint inSizeConstraint) const
        {
            return mBoxLayouter.calculateWidth(inSizeConstraint);
        }

        virtual int calculateHeight(SizeConstraint inSizeConstraint) const
        {
            return mBoxLayouter.calculateHeight(inSizeConstraint);
        }

        virtual Rect clientRect() const
        {
            return Super::clientRect();
        }

        virtual void rebuildChildLayouts()
        {
            return Super::rebuildChildLayouts();
        }

        virtual Orient BoxLayouter_getOrient() const
        {
            return getOrient();
        }

        virtual Align BoxLayouter_getAlign() const
        {
            return getAlign();
        }

        virtual size_t BoxLayouter_getChildCount() const
        {
            return getChildCount();
        }

        virtual const Component * BoxLayouter_getChild(size_t idx) const
        {
            return getChild(idx);
        }

        virtual Component * BoxLayouter_getChild(size_t idx)
        {
            return getChild(idx);
        }

        virtual Rect BoxLayouter_clientRect() const
        {
            return clientRect();
        }

        virtual void BoxLayouter_rebuildChildLayouts()
        {
            rebuildChildLayouts();
        }

    protected:
        BoxLayouter mBoxLayouter;
    };


    class Box : public VirtualComponent,
                public BoxLayouter::ContentProvider
    {
    public:
        typedef VirtualComponent Super;

        Box(Component * inParent, const AttributesMapping & inAttr);

        virtual Orient getOrient() const;

        virtual void setOrient(Orient inOrient);

        virtual Align getAlign() const;

        virtual void rebuildLayout();

        virtual int calculateWidth(SizeConstraint inSizeConstraint) const;

        virtual int calculateHeight(SizeConstraint inSizeConstraint) const;

        virtual Rect clientRect() const;

        virtual const Component * getChild(size_t idx) const;

        virtual Component * getChild(size_t idx);

        virtual void rebuildChildLayouts();

        virtual Orient BoxLayouter_getOrient() const
        {
            return getOrient();
        }

        virtual Align BoxLayouter_getAlign() const
        {
            return getAlign();
        }

        virtual size_t BoxLayouter_getChildCount() const
        {
            return getChildCount();
        }

        virtual const Component * BoxLayouter_getChild(size_t idx) const
        {
            return getChild(idx);
        }

        virtual Component * BoxLayouter_getChild(size_t idx)
        {
            return getChild(idx);
        }

        virtual Rect BoxLayouter_clientRect() const
        {
            return clientRect();
        }

        virtual void BoxLayouter_rebuildChildLayouts()
        {
            rebuildChildLayouts();
        }

    private:
        BoxLayouter mBoxLayouter;
    };

    
    class Separator : public NativeControl
    {
    public:
        typedef NativeControl Super;

        Separator(Component * inParent, const AttributesMapping & inAttr);

        virtual int calculateWidth(SizeConstraint inSizeConstraint) const;

        virtual int calculateHeight(SizeConstraint inSizeConstraint) const;
    };


    class Spacer : public VirtualComponent
    {
    public:
        typedef VirtualComponent Super;

        Spacer(Component * inParent, const AttributesMapping & inAttr);

        virtual int calculateWidth(SizeConstraint inSizeConstraint) const;

        virtual int calculateHeight(SizeConstraint inSizeConstraint) const;
    };


    class MenuButton : public NativeControl
    {
    public:
        typedef NativeControl Super;

        MenuButton(Component * inParent, const AttributesMapping & inAttr);

        virtual int calculateWidth(SizeConstraint inSizeConstraint) const;

        virtual int calculateHeight(SizeConstraint inSizeConstraint) const;
    };


    class VirtualGrid : public VirtualComponent
    {
    public:
        typedef VirtualComponent Super;

        VirtualGrid(Component * inParent, const AttributesMapping & inAttr);

        virtual int calculateWidth(SizeConstraint inSizeConstraint) const;

        virtual int calculateHeight(SizeConstraint inSizeConstraint) const;

        virtual void rebuildLayout();
    };


    class Grid : public NativeControl
    {
    public:
        typedef NativeControl Super;

        Grid(Component * inParent, const AttributesMapping & inAttr);

        virtual int calculateWidth(SizeConstraint inSizeConstraint) const;

        virtual int calculateHeight(SizeConstraint inSizeConstraint) const;

        virtual void rebuildLayout();
    };


    class Rows : public VirtualComponent
    {
    public:
        typedef VirtualComponent Super;

        Rows(Component * inParent, const AttributesMapping & inAttr);

        virtual int calculateWidth(SizeConstraint inSizeConstraint) const;

        virtual int calculateHeight(SizeConstraint inSizeConstraint) const;
    };


    class Row : public VirtualComponent
    {
    public:
        typedef VirtualComponent Super;

        Row(Component * inParent, const AttributesMapping & inAttr);

        virtual int calculateWidth(SizeConstraint inSizeConstraint) const;

        virtual int calculateHeight(SizeConstraint inSizeConstraint) const;
    };


    class Columns : public VirtualComponent
    {
    public:
        typedef VirtualComponent Super;

        Columns(Component * inParent, const AttributesMapping & inAttr);

        virtual int calculateWidth(SizeConstraint inSizeConstraint) const;

        virtual int calculateHeight(SizeConstraint inSizeConstraint) const;
    };


    class Column : public VirtualComponent
    {
    public:
        typedef VirtualComponent Super;

        Column(Component * inParent, const AttributesMapping & inAttr);

        virtual Align getAlign() const;

        virtual int calculateWidth(SizeConstraint inSizeConstraint) const;

        virtual int calculateHeight(SizeConstraint inSizeConstraint) const;
    };


    class RadioGroup : public VirtualBox
    {
    public:
        typedef VirtualBox Super;

        RadioGroup(Component * inParent, const AttributesMapping & inAttr);
    };


    class Radio : public NativeControl
    {
    public:
        typedef NativeControl Super;

        Radio(Component * inParent, const AttributesMapping & inAttr);

        virtual int calculateWidth(SizeConstraint inSizeConstraint) const;

        virtual int calculateHeight(SizeConstraint inSizeConstraint) const;
    };


    class ProgressMeter : public NativeControl,
                          public virtual IntValueController
    {
    public:
        typedef NativeControl Super;

        ProgressMeter(Component * inParent, const AttributesMapping & inAttr);

        // IntValueController methods
        virtual int getValue() const;

        virtual void setValue(int inValue);

        virtual int calculateWidth(SizeConstraint inSizeConstraint) const;

        virtual int calculateHeight(SizeConstraint inSizeConstraint) const;

        bool initAttributeControllers();
    };


    class Deck : public VirtualComponent,
                 public virtual SelectedIndexController
    {
    public:
        typedef VirtualComponent Super;

        Deck(Component * inParent, const AttributesMapping & inAttr);

        // SelectedIndexController methods
        virtual int getSelectedIndex() const;

        virtual void setSelectedIndex(int inSelectedIndex);

        virtual void rebuildLayout();

        virtual int calculateWidth(SizeConstraint inSizeConstraint) const;

        virtual int calculateHeight(SizeConstraint inSizeConstraint) const;

        bool initAttributeControllers();

    private:
        int mSelectedIndex;
    };


    class Tabs : public PhonyComponent
    {
    public:
        typedef PhonyComponent Super;

        Tabs(Component * inParent, const AttributesMapping & inAttr);
    };


    class Tab : public PhonyComponent
    {
    public:
        typedef PhonyComponent Super;

        Tab(Component * inParent, const AttributesMapping & inAttr);
    };


    class TabPanel;
    class Tab;
    class TabPanels : public VirtualComponent
    {
    public:
        typedef VirtualComponent Super;

        TabPanels(Component * inParent, const AttributesMapping & inAttr);

        virtual ~TabPanels();

        void addTabPanel(TabPanel * inPanel);

        virtual void rebuildLayout();

        virtual int calculateWidth(SizeConstraint inSizeConstraint) const;

        virtual int calculateHeight(SizeConstraint inSizeConstraint) const;

        static LRESULT MessageHandler(HWND inHandle, UINT inMessage, WPARAM wParam, LPARAM lParam);

    private:
        void update();

        Tab * getCorrespondingTab(size_t inIndex);
        HWND mParentHandle;
        HWND mTabBarHandle;
        typedef std::map<HWND, TabPanels *> Instances;
        static Instances sInstances;
        WNDPROC mOrigProc;
        size_t mChildCount;
        int mSelectedIndex;
    };


    class TabPanel : public VirtualBox
    {
    public:
        typedef VirtualBox Super;

        TabPanel(Component * inParent, const AttributesMapping & inAttr);

        virtual bool init();
    };


    class GroupBox : public VirtualBox
    {
    public:
        typedef VirtualBox Super;

        GroupBox(Component * inParent, const AttributesMapping & inAttr);

        virtual ~GroupBox();

        void setCaption(const std::string & inLabel);

        virtual void rebuildLayout();

        virtual Orient getOrient() const;

        virtual int calculateWidth(SizeConstraint inSizeConstraint) const;

        virtual int calculateHeight(SizeConstraint inSizeConstraint) const;

        virtual size_t getChildCount() const;

        virtual const Component * getChild(size_t idx) const;

        virtual Component * getChild(size_t idx);

        virtual Rect clientRect() const;

    private:
        HWND mGroupBoxHandle;
        int mMarginLeft;
        int mMarginTop;
        int mMarginRight;
        int mMarginBottom;
    };


    class Caption : public VirtualComponent
    {
    public:
        typedef VirtualComponent Super;

        Caption(Component * inParent, const AttributesMapping & inAttr);

        virtual int calculateWidth(SizeConstraint inSizeConstraint) const;

        virtual int calculateHeight(SizeConstraint inSizeConstraint) const;

        virtual bool init();
    };


    typedef GenericNode<std::string, ContainerPolicy_Vector, PointerPolicy_Normal_NoOwnership> TreeItemInfo;


    class TreeCell;
    class Tree : public NativeControl
    {
    public:
        typedef NativeControl Super;

        Tree(Component * inParent, const AttributesMapping & inAttr);

        virtual int calculateWidth(SizeConstraint inSizeConstraint) const;

        virtual int calculateHeight(SizeConstraint inSizeConstraint) const;

        void addInfo(const TreeItemInfo & inInfo);

        HTREEITEM addInfo(HTREEITEM inRoot, HTREEITEM inPrev, const TreeItemInfo & inInfo);

        virtual bool init();
    };


    class TreeItem;
    class TreeChildren : public PhonyComponent
    {
    public:
        typedef PhonyComponent Super;

        TreeChildren(Component * inParent, const AttributesMapping & inAttr);

        virtual int calculateWidth(SizeConstraint inSizeConstraint) const;

        virtual int calculateHeight(SizeConstraint inSizeConstraint) const;
    };


    class TreeRow;
    class TreeItem : public PhonyComponent
    {
    public:
        typedef PhonyComponent Super;

        TreeItem(Component * inParent, const AttributesMapping & inAttr);

        virtual bool init();

        const TreeItemInfo & itemInfo() const
        {
            return mItemInfo;
        }

        TreeItemInfo & itemInfo()
        {
            return mItemInfo;
        }

        bool isOpened() const;

        virtual int calculateWidth(SizeConstraint inSizeConstraint) const;

        virtual int calculateHeight(SizeConstraint inSizeConstraint) const;

    private:
        TreeItemInfo mItemInfo;
    };


    class TreeCols : public PhonyComponent
    {
    public:
        typedef PhonyComponent Super;

        TreeCols(Component * inParent, const AttributesMapping & inAttr);
    };


    class TreeCol : public PhonyComponent
    {
    public:
        typedef PhonyComponent Super;

        TreeCol(Component * inParent, const AttributesMapping & inAttr);
    };


    class TreeCell;
    class TreeRow : public PhonyComponent
    {
    public:
        typedef PhonyComponent Super;

        TreeRow(Component * inParent, const AttributesMapping & inAttr);

        virtual int calculateWidth(SizeConstraint inSizeConstraint) const;

        virtual int calculateHeight(SizeConstraint inSizeConstraint) const;
    };


    class TreeCell : public PhonyComponent,
                     public LabelController
    {
    public:
        typedef PhonyComponent Super;

        TreeCell(Component * inParent, const AttributesMapping & inAttr);

        virtual bool initAttributeControllers();

        virtual std::string getLabel() const;

        virtual void setLabel(const std::string & inLabel);

        virtual int calculateWidth(SizeConstraint inSizeConstraint) const;

        virtual int calculateHeight(SizeConstraint inSizeConstraint) const;

    private:
        std::string mLabel;
    };



    class Statusbar : public NativeControl,
                      public BoxLayouter::ContentProvider
    {
    public:
        typedef NativeControl Super;

        Statusbar(Component * inParent, const AttributesMapping & inAttr);

        virtual bool initAttributeControllers();

        virtual int calculateWidth(SizeConstraint inSizeConstraint) const;

        virtual int calculateHeight(SizeConstraint inSizeConstraint) const;

        virtual Orient getOrient() const;

        virtual Align getAlign() const;

        virtual Rect clientRect() const;

        virtual void rebuildChildLayouts();

        virtual void rebuildLayout();

        virtual Orient BoxLayouter_getOrient() const
        {
            return getOrient();
        }

        virtual Align BoxLayouter_getAlign() const
        {
            return getAlign();
        }

        virtual size_t BoxLayouter_getChildCount() const
        {
            return getChildCount();
        }

        virtual const Component * BoxLayouter_getChild(size_t idx) const
        {
            return getChild(idx);
        }

        virtual Component * BoxLayouter_getChild(size_t idx)
        {
            return getChild(idx);
        }

        virtual Rect BoxLayouter_clientRect() const
        {
            return clientRect();
        }

        virtual void BoxLayouter_rebuildChildLayouts()
        {
            rebuildChildLayouts();
        }

    private:
        BoxLayouter mBoxLayouter;
    };



    class StatusbarPanel : public NativeControl
    {
    public:
        typedef NativeControl Super;

        StatusbarPanel(Component * inParent, const AttributesMapping & inAttr);

        virtual bool initAttributeControllers();

        virtual int calculateWidth(SizeConstraint inSizeConstraint) const;

        virtual int calculateHeight(SizeConstraint inSizeConstraint) const;
    };


} // namespace XULWin


#endif // COMPONENTS_H_INCLUDED
