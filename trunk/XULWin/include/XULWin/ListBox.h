#ifndef LISTBOX_H_INCLUDED
#define LISTBOX_H_INCLUDED


#include "XULWin/NativeControl.h"
#include "XULWin/EventListener.h"
#include "XULWin/VirtualComponent.h"
#include "XULWin/PhonyComponent.h"


namespace XULWin
{


    class ListBox : public NativeControl,
                    public RowsController
    {
    public:
        typedef NativeControl Super;

        ListBox(Component * inParent, const AttributesMapping & inAttr);

        virtual bool initAttributeControllers();

        virtual int getRows() const;

        virtual void setRows(int inRows);

        virtual int calculateWidth(SizeConstraint inSizeConstraint) const;

        virtual int calculateHeight(SizeConstraint inSizeConstraint) const;

        virtual void onChildAdded(Component * inChild);

    private:
        Fallible<int> mRows;
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
