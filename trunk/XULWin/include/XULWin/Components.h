#ifndef COMPONENTS_H_INCLUDED
#define COMPONENTS_H_INCLUDED


#include "XULWin/AttributeController.h"
#include "XULWin/BoxLayouter.h"
#include "XULWin/Conversions.h"
#include "XULWin/Component.h"
#include "XULWin/Element.h"
#include "XULWin/Enums.h"
#include "XULWin/EventListener.h"
#include "XULWin/Fallible.h"
#include "XULWin/GdiplusLoader.h"
#include "XULWin/Node.h"
#include "XULWin/Layout.h"
#include "XULWin/StyleController.h"
#include "XULWin/Toolbar.h"
#include "XULWin/ToolbarItem.h"
#include "XULWin/UniqueId.h"
#include "XULWin/Window.h"
#include "XULWin/Windows.h"
#include <boost/function.hpp>
#include <boost/noncopyable.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <map>
#include <string>
#include <CommCtrl.h>


namespace XULWin
{


    /**
     * Dialog
     *
     * Represents a XUL dialog window.
     * In reality it is a Window with some customizations for Dialog-like behavior.
     */
    class Dialog : public NativeComponent,
                   public BoxLayouter::ContentProvider,
                   public virtual TitleController
    {
    public:
        typedef NativeComponent Super;

        static void Register(HMODULE inModuleHandle);

        Dialog(Component * inParent, const AttributesMapping & inAttributesMapping);

        virtual ~Dialog();

        // BoxLayouter
        virtual Orient getOrient() const;

        // BoxLayouter
        virtual Align getAlign() const;

        // TitleController methods
        virtual std::string getTitle() const;

        virtual void setTitle(const std::string & inTitle);

        virtual const Component * getChild(size_t idx) const;

        virtual Component * getChild(size_t idx);

        virtual void rebuildChildLayouts()
        {
            return Super::rebuildChildLayouts();
        }

        DialogResult showModal(Window * inInvoker);

        LRESULT endModal(DialogResult inDialogResult);

        virtual void move(int x, int y, int w, int h);

        virtual void rebuildLayout();

        virtual int calculateWidth(SizeConstraint inSizeConstraint) const;

        virtual int calculateHeight(SizeConstraint inSizeConstraint) const;

        virtual Rect clientRect() const;

        virtual Rect windowRect() const;

        virtual bool initAttributeControllers();

        virtual bool initStyleControllers();

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

        virtual LRESULT handleMessage(UINT inMessage, WPARAM wParam, LPARAM lParam);

#ifndef SWIG
        static LRESULT CALLBACK MessageHandler(HWND hWnd, UINT inMessage, WPARAM wParam, LPARAM lParam);
#endif
    private:
        // Invoker is the stored parameter for showModal.
        Window * mInvoker;
        BoxLayouter mBoxLayouter;
        DialogResult mDialogResult;
    };


    /**
     * NativeControl
     *
     * A native control is a control that is not a Window
     * or a Dialog and that maps to a native HWND.
     * It is the base class for native WinAPI controls.
     */
    class NativeControl : public NativeComponent
    {
    public:
        typedef NativeComponent Super;

        NativeControl(Component * inParent, const AttributesMapping & inAttributesMapping, LPCTSTR inClassName, DWORD inExStyle, DWORD inStyle);

        // use this constructor if you want to provide your own handle later using NativeControl::setHandle
        NativeControl(Component * inParent, const AttributesMapping & inAttributesMapping);

        virtual ~NativeControl();

        bool initStyleControllers();

        virtual void rebuildLayout();

        virtual Rect clientRect() const;

        virtual void move(int x, int y, int w, int h);

        // Gets a NativeComponent object from this object. This
        // is only needed in constructors of s, because
        // they need to know which is their native parent handle object.
        // If this is a NativeComponent, return this.
        // If this is a VirtualComponent, return first parent that is a NativeComponent.
        // If this is a Decorator, resolve until a NativeComponent is found.
        static NativeComponent * GetThisOrParent(Component * inElement);
        static const NativeComponent * GetThisOrParent(const Component * inElement);
    };


    /**
     * VirtualComponent
     *
     * A virtual component is a widget that does not map to a native HWND.
     * Layout elements (box, grid, deck ..) are implemented as virtual components.
     */
    class VirtualComponent : public ConcreteComponent
    {
    public:
        typedef ConcreteComponent Super;

        VirtualComponent(Component * inParent, const AttributesMapping & inAttributesMapping);

        virtual ~VirtualComponent();

        virtual bool initAttributeControllers();

        virtual bool initStyleControllers();

        virtual int calculateWidth(SizeConstraint inSizeConstraint) const = 0;

        virtual int calculateHeight(SizeConstraint inSizeConstraint) const = 0;

        virtual void rebuildLayout();

        virtual Rect clientRect() const;

        virtual void move(int x, int y, int w, int h);

        virtual LRESULT handleMessage(UINT inMessage, WPARAM wParam, LPARAM lParam);

    protected:
        Rect mRect;
    };


    /**
     * DummyComponent
     *
     * A passive component represents XUL elements that do not map to
     * a native HWND and that also do not ask for layout space. Often
     * they are not a widget at all, for example "listcols".
     */
    class DummyComponent : public VirtualComponent
    {
    public:
        typedef VirtualComponent Super;

        DummyComponent(Component * inParent, const AttributesMapping & inAttributesMapping);

        virtual ~DummyComponent();

        virtual int calculateWidth(SizeConstraint inSizeConstraint) const
        {
            return 0;
        }

        virtual int calculateHeight(SizeConstraint inSizeConstraint) const
        {
            return 0;
        }
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

        Button(Component * inParent, const AttributesMapping & inAttributesMapping);

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

        Description(Component * inParent, const AttributesMapping & inAttributesMapping);

        // StringValueController methods
        virtual std::string getValue() const;

        virtual void setValue(const std::string & inStringValue);

        virtual bool initAttributeControllers();

        virtual int calculateWidth(SizeConstraint inSizeConstraint) const;

        virtual int calculateHeight(SizeConstraint inSizeConstraint) const;
    };


    class TextBox : public NativeControl,
                    public virtual StringValueController,
                    public virtual ReadOnlyController,
                    public virtual RowsController
    {
    public:
        typedef NativeControl Super;

        TextBox(Component * inParent, const AttributesMapping & inAttributesMapping);

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
        static DWORD GetFlags(const AttributesMapping & inAttributesMapping);
    };


    class CheckBox : public NativeControl,
                     public virtual CheckedController
    {
    public:
        typedef NativeControl Super;

        CheckBox(Component * inParent, const AttributesMapping & inAttributesMapping);

        // CheckedController methods
        virtual bool isChecked() const;

        virtual void setChecked(bool inChecked);

        virtual bool initAttributeControllers();

        virtual int calculateWidth(SizeConstraint inSizeConstraint) const;

        virtual int calculateHeight(SizeConstraint inSizeConstraint) const;
    };


    class VirtualBox : public VirtualComponent,
                       public BoxLayouter::ContentProvider
    {
    public:
        typedef VirtualComponent Super;

        VirtualBox(Component * inParent, const AttributesMapping & inAttributesMapping);

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


    class Box : public NativeControl,
                public BoxLayouter::ContentProvider
    {
    public:
        typedef NativeControl Super;

        Box(Component * inParent, const AttributesMapping & inAttributesMapping);

        virtual Orient getOrient() const;

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


    class MenuPopupContainer
    {
    public:
        virtual void showPopupMenu(RECT inToolbarButtonRect) = 0;
    };


    class MenuList : public NativeControl
    {
    public:
        typedef NativeControl Super;

        MenuList(Component * inParent, const AttributesMapping & inAttributesMapping);

        virtual bool init();

        virtual int calculateWidth(SizeConstraint inSizeConstraint) const;

        virtual int calculateHeight(SizeConstraint inSizeConstraint) const;

        virtual void move(int x, int y, int w, int h);

        virtual void onContentChanged();

    private:
        void fillComboBox();
    };


    class Separator : public NativeControl
    {
    public:
        typedef NativeControl Super;

        Separator(Component * inParent, const AttributesMapping & inAttributesMapping);

        virtual int calculateWidth(SizeConstraint inSizeConstraint) const;

        virtual int calculateHeight(SizeConstraint inSizeConstraint) const;
    };


    class Spacer : public VirtualComponent
    {
    public:
        typedef VirtualComponent Super;

        Spacer(Component * inParent, const AttributesMapping & inAttributesMapping);

        virtual int calculateWidth(SizeConstraint inSizeConstraint) const;

        virtual int calculateHeight(SizeConstraint inSizeConstraint) const;
    };


    class MenuButton : public NativeControl
    {
    public:
        typedef NativeControl Super;

        MenuButton(Component * inParent, const AttributesMapping & inAttributesMapping);

        virtual int calculateWidth(SizeConstraint inSizeConstraint) const;

        virtual int calculateHeight(SizeConstraint inSizeConstraint) const;
    };


    class VirtualGrid : public VirtualComponent
    {
    public:
        typedef VirtualComponent Super;

        VirtualGrid(Component * inParent, const AttributesMapping & inAttributesMapping);

        virtual int calculateWidth(SizeConstraint inSizeConstraint) const;

        virtual int calculateHeight(SizeConstraint inSizeConstraint) const;

        virtual void rebuildLayout();
    };


    class Grid : public NativeControl
    {
    public:
        typedef NativeControl Super;

        Grid(Component * inParent, const AttributesMapping & inAttributesMapping);

        virtual int calculateWidth(SizeConstraint inSizeConstraint) const;

        virtual int calculateHeight(SizeConstraint inSizeConstraint) const;

        virtual void rebuildLayout();
    };


    class Rows : public VirtualComponent
    {
    public:
        typedef VirtualComponent Super;

        Rows(Component * inParent, const AttributesMapping & inAttributesMapping);

        virtual int calculateWidth(SizeConstraint inSizeConstraint) const;

        virtual int calculateHeight(SizeConstraint inSizeConstraint) const;
    };


    class Row : public VirtualComponent
    {
    public:
        typedef VirtualComponent Super;

        Row(Component * inParent, const AttributesMapping & inAttributesMapping);

        virtual int calculateWidth(SizeConstraint inSizeConstraint) const;

        virtual int calculateHeight(SizeConstraint inSizeConstraint) const;
    };


    class Columns : public VirtualComponent
    {
    public:
        typedef VirtualComponent Super;

        Columns(Component * inParent, const AttributesMapping & inAttributesMapping);

        virtual int calculateWidth(SizeConstraint inSizeConstraint) const;

        virtual int calculateHeight(SizeConstraint inSizeConstraint) const;
    };


    class Column : public VirtualComponent
    {
    public:
        typedef VirtualComponent Super;

        Column(Component * inParent, const AttributesMapping & inAttributesMapping);

        virtual Align getAlign() const;

        virtual int calculateWidth(SizeConstraint inSizeConstraint) const;

        virtual int calculateHeight(SizeConstraint inSizeConstraint) const;
    };


    class RadioGroup : public VirtualBox
    {
    public:
        typedef VirtualBox Super;

        RadioGroup(Component * inParent, const AttributesMapping & inAttributesMapping);
    };


    class Radio : public NativeControl
    {
    public:
        typedef NativeControl Super;

        Radio(Component * inParent, const AttributesMapping & inAttributesMapping);

        virtual int calculateWidth(SizeConstraint inSizeConstraint) const;

        virtual int calculateHeight(SizeConstraint inSizeConstraint) const;
    };


    class ProgressMeter : public NativeControl,
                          public virtual IntValueController
    {
    public:
        typedef NativeControl Super;

        ProgressMeter(Component * inParent, const AttributesMapping & inAttributesMapping);

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

        Deck(Component * inParent, const AttributesMapping & inAttributesMapping);

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


    class Tabs : public DummyComponent
    {
    public:
        typedef DummyComponent Super;

        Tabs(Component * inParent, const AttributesMapping & inAttributesMapping);
    };


    class Tab : public DummyComponent
    {
    public:
        typedef DummyComponent Super;

        Tab(Component * inParent, const AttributesMapping & inAttributesMapping);
    };


    class TabPanel;
    class Tab;
    class TabPanels : public VirtualComponent
    {
    public:
        typedef VirtualComponent Super;

        TabPanels(Component * inParent, const AttributesMapping & inAttributesMapping);

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

        TabPanel(Component * inParent, const AttributesMapping & inAttributesMapping);

        virtual bool init();
    };


    class GroupBox : public VirtualBox
    {
    public:
        typedef VirtualBox Super;

        GroupBox(Component * inParent, const AttributesMapping & inAttributesMapping);

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

        Caption(Component * inParent, const AttributesMapping & inAttributesMapping);

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

        Tree(Component * inParent, const AttributesMapping & inAttributesMapping);

        virtual int calculateWidth(SizeConstraint inSizeConstraint) const;

        virtual int calculateHeight(SizeConstraint inSizeConstraint) const;

        void addInfo(const TreeItemInfo & inInfo);

        HTREEITEM addInfo(HTREEITEM inRoot, HTREEITEM inPrev, const TreeItemInfo & inInfo);

        virtual bool init();
    };


    class TreeItem;
    class TreeChildren : public DummyComponent
    {
    public:
        typedef DummyComponent Super;

        TreeChildren(Component * inParent, const AttributesMapping & inAttributesMapping);

        virtual int calculateWidth(SizeConstraint inSizeConstraint) const;

        virtual int calculateHeight(SizeConstraint inSizeConstraint) const;
    };


    class TreeRow;
    class TreeItem : public DummyComponent
    {
    public:
        typedef DummyComponent Super;

        TreeItem(Component * inParent, const AttributesMapping & inAttributesMapping);

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


    class TreeCols : public DummyComponent
    {
    public:
        typedef DummyComponent Super;

        TreeCols(Component * inParent, const AttributesMapping & inAttributesMapping);
    };


    class TreeCol : public DummyComponent
    {
    public:
        typedef DummyComponent Super;

        TreeCol(Component * inParent, const AttributesMapping & inAttributesMapping);
    };


    class TreeCell;
    class TreeRow : public DummyComponent
    {
    public:
        typedef DummyComponent Super;

        TreeRow(Component * inParent, const AttributesMapping & inAttributesMapping);

        virtual int calculateWidth(SizeConstraint inSizeConstraint) const;

        virtual int calculateHeight(SizeConstraint inSizeConstraint) const;
    };


    class TreeCell : public DummyComponent,
                     public LabelController
    {
    public:
        typedef DummyComponent Super;

        TreeCell(Component * inParent, const AttributesMapping & inAttributesMapping);

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

        Statusbar(Component * inParent, const AttributesMapping & inAttributesMapping);

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

        StatusbarPanel(Component * inParent, const AttributesMapping & inAttributesMapping);

        virtual bool initAttributeControllers();

        virtual int calculateWidth(SizeConstraint inSizeConstraint) const;

        virtual int calculateHeight(SizeConstraint inSizeConstraint) const;
    };


    class Toolbar : public NativeControl,
                    public Windows::Toolbar::EventHandler,
                    public GdiplusLoader
    {
    public:
        typedef NativeControl Super;

        Toolbar(Component * inParent, const AttributesMapping & inAttributesMapping);

        virtual ~Toolbar();

        virtual bool init();

        virtual int calculateWidth(SizeConstraint inSizeConstraint) const;

        virtual int calculateHeight(SizeConstraint inSizeConstraint) const;

        virtual void rebuildLayout();

        // ToolbarElement::EventHandler methods
        virtual void onRequestFocus() {}

        boost::shared_ptr<Windows::Toolbar> nativeToolbar() const
        {
            return mToolbar;
        }

    private:
        boost::shared_ptr<Windows::Toolbar> mToolbar;
    };


    class ToolbarButton : public VirtualComponent,
                          public Windows::ToolbarDropDown::EventHandler,
                          public MenuPopupContainer,
                          public virtual DisabledController,
                          public virtual LabelController,
                          public virtual CSSListStyleImageController
    {
    public:
        typedef VirtualComponent Super;

        ToolbarButton(Component * inParent, const AttributesMapping & inAttributesMapping);

        virtual bool initAttributeControllers();

        virtual bool initStyleControllers();

        virtual int calculateWidth(SizeConstraint inSizeConstraint) const;

        virtual int calculateHeight(SizeConstraint inSizeConstraint) const;

        // From ToolbarDropDown::EventHandler
        virtual void showToolbarMenu(RECT inToolbarButtonRect);

        // From MenuPopupContainer
        virtual void showPopupMenu(RECT inToolbarButtonRect);

        virtual std::string getLabel() const;

        virtual void setLabel(const std::string & inLabel);

        virtual bool isDisabled() const;

        virtual void setDisabled(bool inDisabled);

        virtual void setCSSListStyleImage(const std::string & inURL);

        virtual const std::string & getCSSListStyleImage() const;

        Windows::ConcreteToolbarItem * nativeItem()
        {
            return mButton;
        }

    private:
        Windows::ConcreteToolbarItem * mButton;
        bool mDisabled;
        std::string mLabel;
        std::string mCSSListStyleImage;
    };


} // namespace XULWin


#endif // COMPONENTS_H_INCLUDED
