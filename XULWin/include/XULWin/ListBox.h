#ifndef LISTBOX_H_INCLUDED
#define LISTBOX_H_INCLUDED


#include "XULWin/NativeControl.h"
#include "XULWin/EventListener.h"
#include "XULWin/VirtualComponent.h"
#include "XULWin/PhonyComponent.h"
#include "XULWin/WindowsListView.h"
#include <boost/scoped_ptr.hpp>


namespace XULWin
{

    class ListBoxImpl;

    
    namespace WinAPI
    {
        class ListBox;
        class ListView;
    }


    class ListBox : public NativeControl,
                    public RowsController
    {
    public:
        typedef NativeControl Super;

        ListBox(Component * inParent, const AttributesMapping & inAttr);

        virtual ~ListBox();

        virtual bool init();

        virtual bool initAttributeControllers();

        virtual int getRows() const;

        virtual void setRows(int inRows);

        virtual int calculateWidth(SizeConstraint inSizeConstraint) const;

        virtual int calculateHeight(SizeConstraint inSizeConstraint) const;

    private:
        bool isListView() const;

        Fallible<int> mRows;
        boost::scoped_ptr<ListBoxImpl> mListBoxImpl;
    };

    
    /**
     * ListBoxImpl is the base class for either a Window list box or a Windows list view.
     */
    class ListBoxImpl
    {
    public:
        ListBoxImpl(ListBox * inListBox);

        virtual ~ListBoxImpl() = 0;

        HWND handle() const;

    private:
        ListBox * mListBox;
    };


    // Encapsulates a WinAPI list box.
    class ListBoxImpl_ListBox : public ListBoxImpl
    {
    public:
        ListBoxImpl_ListBox(ListBox * inListBox);

        virtual ~ListBoxImpl_ListBox();

    private:
        boost::scoped_ptr<WinAPI::ListBox> mWinAPI_ListBox;
    };



    // Encapsulates a WinAPI list view.
    class ListBoxImpl_ListView : public ListBoxImpl
    {
    public:
        ListBoxImpl_ListView(ListBox * inListBox);

        virtual ~ListBoxImpl_ListView();

    private:
        boost::scoped_ptr<WinAPI::ListView> mWinAPI_ListView;
    };


    class ListItem : public VirtualComponent,
                     public LabelController,
                     public SelectedController
    {
    public:
        typedef VirtualComponent Super;

        ListItem(Component * inParent, const AttributesMapping & inAttr);

        virtual bool init();

        virtual bool initAttributeControllers();

        virtual std::string getLabel() const;

        virtual void setLabel(const std::string & inLabel);

        virtual bool isSelected() const;

        virtual void setSelected(bool inSelected);

        virtual int calculateWidth(SizeConstraint inSizeConstraint) const;

        virtual int calculateHeight(SizeConstraint inSizeConstraint) const;

    private:
        std::string mLabel;
        bool mSelected;
    };


    class ListHeader : public PhonyComponent,
                       public LabelController
    {
    public:
        typedef PhonyComponent Super;

        ListHeader(Component * inParent, const AttributesMapping & inAttr);

        virtual bool init();

        bool initAttributeControllers();

        virtual std::string getLabel() const;

        virtual void setLabel(const std::string & inLabel);

    private:
        std::string mLabel;
    };


    class ListHead : public VirtualComponent
    {
    public:
        typedef VirtualComponent Super;

        ListHead(Component * inParent, const AttributesMapping & inAttr);

        virtual bool init();

        bool initAttributeControllers();

        int calculateWidth(SizeConstraint inSizeConstraint) const;

        int calculateHeight(SizeConstraint inSizeConstraint) const;

        virtual void onChildAdded(Component * inChild);
    };


    class ListCols : public PhonyComponent
    {
    public:
        typedef PhonyComponent Super;

        ListCols(Component * inParent, const AttributesMapping & inAttr);

        virtual bool init();

        bool initAttributeControllers();

        int calculateWidth(SizeConstraint inSizeConstraint) const;

        int calculateHeight(SizeConstraint inSizeConstraint) const;
    };


    class ListCol : public PhonyComponent
    {
    public:
        typedef PhonyComponent Super;

        ListCol(Component * inParent, const AttributesMapping & inAttr);

        virtual bool init();

        int calculateWidth(SizeConstraint inSizeConstraint) const;

        int calculateHeight(SizeConstraint inSizeConstraint) const;
    };


    class ListView : public NativeControl
    {
    public:
        typedef NativeControl Super;

        ListView(Component * inParent,
                 const AttributesMapping & inAttr);

        virtual ~ListView();

        virtual bool init();

        bool initAttributeControllers();

        int calculateWidth(SizeConstraint inSizeConstraint) const;

        int calculateHeight(SizeConstraint inSizeConstraint) const;

        virtual void onChildAdded(Component * inChild);

        void addListHeader(ListHeader * inListHeader);

    private:
        LRESULT handleGetDispInfo(WPARAM wParam, LPARAM lParam);

        ScopedEventListener mEventListener;
    };


    class ListCell : public VirtualComponent,
                     public virtual LabelController
    {
    public:
        typedef VirtualComponent Super;

        ListCell(Component * inParent, const AttributesMapping & inAttr);

        virtual bool initAttributeControllers();

        int calculateWidth(SizeConstraint inSizeConstraint) const;

        int calculateHeight(SizeConstraint inSizeConstraint) const;

        virtual std::string getLabel() const
        {
            return mLabel;
        }

        virtual void setLabel(const std::string & inLabel)
        {
            mLabel = inLabel;
        }

    private:
        std::string mLabel;
    };

} // namespace XULWin


#endif // LISTBOX_H_INCLUDED
