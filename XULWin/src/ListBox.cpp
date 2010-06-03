#include "XULWin/ListBox.h"
#include "XULWin/Element.h"
#include "XULWin/ErrorReporter.h"
#include "XULWin/MarginDecorator.h"
#include "XULWin/Unicode.h"
#include "XULWin/WinUtils.h"
#include <boost/bind.hpp>


namespace XULWin
{
    
    Component * CreateListBox(Component * inComponent, const AttributesMapping & inAttr)
    {
        return new MarginDecorator(new ListBox(inComponent, inAttr));
    }


    ListBox::ListBox(Component * inParent, const AttributesMapping & inAttr) :
        NativeControl(inParent, inAttr, TEXT("LISTBOX"), WS_EX_CLIENTEDGE, 0),
        mRows(0)
    {
        mRows.setInvalid();
    }


    void ListBox::onChildAdded(Component * inChild)
    {
        if (XMLListBox * listBox = el()->downcast<XMLListBox>())
        {
            if (ListItem * item = inChild->downcast<ListItem>())
            {
                WinAPI::addStringToListBox(handle(), item->getLabel());
            }
        }
    }


    bool ListBox::initAttributeControllers()
    {
        // FIXME: does the ListBox implement the "label" attribute?
        //setAttributeController<LabelController>(this);
        setAttributeController<RowsController>(this);
        return Super::initAttributeControllers();
    }


    int ListBox::getRows() const
    {
        return mRows.or(1);
    }


    void ListBox::setRows(int inRows)
    {
        mRows = inRows;
    }


    int ListBox::calculateWidth(SizeConstraint inSizeConstraint) const
    {
        return calculateMaxChildWidth(inSizeConstraint);
    }


    int ListBox::calculateHeight(SizeConstraint inSizeConstraint) const
    {
        int result = 0;
        int itemCount = mRows.or(getChildCount());
        for (size_t idx = 0; idx != itemCount; ++idx)
        {
            if (idx < getChildCount())
            {
                result += getChild(idx)->calculateHeight(inSizeConstraint);
            }
            else
            {
                result += getChild(0)->calculateHeight(inSizeConstraint);
            }
        }
        int extraHeight = WinAPI::getSizeDifferenceBetweenWindowRectAndClientRect(handle()).cy;
        return result + extraHeight;
    }
    
    Component * CreateListCell(XULWin::Component * inParent, const AttributesMapping & inAttr)
    {
        return new Decorator(new ListCell(inParent, inAttr));
    }


    ListCell::ListCell(Component * inParent, const AttributesMapping & inAttr) :
        VirtualComponent(inParent, inAttr)
    {
    }


    bool ListCell::initAttributeControllers()
    {
        setAttributeController<LabelController>(this);
        return Super::initAttributeControllers();
    }


    int ListCell::calculateWidth(SizeConstraint inSizeConstraint) const
    {
        //int result = 0;
        //if (NativeComponent * comp = NativeControl::FindNativeParent(parent()))
        //{
        //    int w = WinAPI::getTextSize(comp->handle(), getLabel()).cx + Defaults::listViewColumnTextPadding();
        //    if (w > result)
        //    {
        //        result = w;
        //    }
        //}
        //return result;

        //FIXME! Implement this correctly!
        return 80;
    }


    int ListCell::calculateHeight(SizeConstraint inSizeConstraint) const
    {
        return 1;
    }
    
    Component * CreateListCol(XULWin::Component * inParent, const AttributesMapping & inAttr)
    {
        return new Decorator(new ListCol(inParent, inAttr));
    }


    ListCol::ListCol(Component * inParent, const AttributesMapping & inAttr) :
        Super(inParent, inAttr)
    {
    }


    int ListCol::calculateWidth(SizeConstraint inSizeConstraint) const
    {
        int res = 1;
        // XUL Hierarchy:
        // listbox/listcols/listcol
        // listbox/listitem/listcell
        std::vector<XMLListItem *> listItems;
        parent()->parent()->el()->getElementsByType<XMLListItem>(listItems);
        const int colIdx = getIndex();
        for (size_t idx = 0; idx != listItems.size(); ++idx)
        {
            if (ListCell * cell = listItems[idx]->component()->getChild(colIdx)->downcast<ListCell>())
            {
                res = std::max<int>(res, cell->calculateWidth(inSizeConstraint));
            }
        }
        return res;
    }


    int ListCol::calculateHeight(SizeConstraint inSizeConstraint) const
    {
        return 1;
    }


    bool ListCol::init()
    {
        return Super::init();
    }

    Component * CreateListCols(XULWin::Component * inParent, const AttributesMapping & inAttr)
    {
        return new Decorator(new ListCols(inParent, inAttr));
    }


    ListCols::ListCols(Component * inParent, const AttributesMapping & inAttr) :
        Super(inParent, inAttr)
    {
    }


    bool ListCols::init()
    {
        return Super::init();
    }


    bool ListCols::initAttributeControllers()
    {
        return Super::initAttributeControllers();
    }


    int ListCols::calculateWidth(SizeConstraint inSizeConstraint) const
    {
        return 1;
    }


    int ListCols::calculateHeight(SizeConstraint inSizeConstraint) const
    {
        return 1;
    }

    Component * CreateListHead(XULWin::Component * inParent, const AttributesMapping & inAttr)
    {
        return new Decorator(new ListHead(inParent, inAttr));
    }


    ListHead::ListHead(Component * inParent, const AttributesMapping & inAttr) :
        Super(inParent, inAttr)
    {
    }


    bool ListHead::init()
    {
        return Super::init();
    }


    bool ListHead::initAttributeControllers()
    {
        return Super::initAttributeControllers();
    }


    int ListHead::calculateWidth(SizeConstraint inSizeConstraint) const
    {
        return 1;
    }


    int ListHead::calculateHeight(SizeConstraint inSizeConstraint) const
    {
        return 1;
    }


    void ListHead::onChildAdded(Component * inChild)
    {
        if (ListView * listViewParent = parent()->downcast<ListView>())
        {
            if (ListHeader * listHeader = inChild->downcast<ListHeader>())
            {
                listViewParent->addListHeader(listHeader);
            }
        }
        else
        {
            ReportError("Invalid container for XMLListHeader!");
        }
    }

    Component * CreateListHeader(XULWin::Component * inParent, const AttributesMapping & inAttr)
    {
        return new Decorator(new ListHeader(inParent, inAttr));
    }


    ListHeader::ListHeader(Component * inParent, const AttributesMapping & inAttr) :
        Super(inParent, inAttr)
    {
    }


    bool ListHeader::init()
    {
        return Super::init();
    }


    bool ListHeader::initAttributeControllers()
    {
        setAttributeController<LabelController>(this);
        return Super::initAttributeControllers();
    }


    std::string ListHeader::getLabel() const
    {
        return mLabel;
    }


    void ListHeader::setLabel(const std::string & inLabel)
    {
        mLabel = inLabel;
    }

    Component * CreateListItem(Component * inComponent, const AttributesMapping & inAttr)
    {
        return new MarginDecorator(new ListItem(inComponent, inAttr));
    }


    ListItem::ListItem(Component * inParent, const AttributesMapping & inAttr) :
        VirtualComponent(inParent, inAttr),
        mSelected(false)
    {
    }


    bool ListItem::init()
    {
        // XUL hierarchy:
        // listbox/listitem/listcell
        //    ^---> listbox here has ListView object (not ListBox)
        if (ListView * listView = parent()->downcast<ListView>())
        {
            LVITEM lvItem;
            lvItem.mask = LVIF_TEXT | LVIF_PARAM | LVIF_STATE;
            lvItem.state = 0;
            lvItem.stateMask = 0;
            lvItem.iSubItem = 0;
            lvItem.lParam = (LPARAM)0;
            std::vector<XMLListCell *> listCells;
            el()->getElementsByType<XMLListCell>(listCells);

            if (!listCells.empty())
            {
                static std::vector<std::wstring> labels;
                labels.clear();

                for (size_t idx = 0; idx != listCells.size(); ++idx)
                {
                    labels.push_back(ToUTF16(listCells[idx]->component()->downcast<ListCell>()->getLabel()));
                }
                lvItem.lParam = (LPARAM)labels[0].c_str();
                lvItem.iItem = getIndex();
                lvItem.pszText = LPSTR_TEXTCALLBACK;
                if (-1 == ListView_InsertItem(listView->handle(), &lvItem))
                {
                    ReportError("Inserting item into list view failed. Reason: " + WinAPI::getLastError(::GetLastError()));
                }
            }
        }
        return Super::init();
    }


    std::string ListItem::getLabel() const
    {
        return mLabel;
    }


    void ListItem::setLabel(const std::string & inLabel)
    {
        mLabel = inLabel;
    }


    bool ListItem::isSelected() const
    {
        return mSelected;
    }


    void ListItem::setSelected(bool inSelected)
    {
        mSelected = inSelected;
    }


    bool ListItem::initAttributeControllers()
    {
        setAttributeController<LabelController>(this);
        setAttributeController<SelectedController>(this);
        return Super::initAttributeControllers();
    }


    int ListItem::calculateWidth(SizeConstraint inSizeConstraint) const
    {
        static int cMargin = 4;
        int result = 0;
        if (ListBox * listBox = parent()->downcast<ListBox>())
        {
            int width = WinAPI::getTextSize(listBox->handle(), getLabel()).cx;
            int extraWidth = WinAPI::getSizeDifferenceBetweenWindowRectAndClientRect(listBox->handle()).cx;
            result = width + extraWidth + cMargin;
        }
        return result;
    }


    int ListItem::calculateHeight(SizeConstraint inSizeConstraint) const
    {
        if (ListBox * listBox = parent()->downcast<ListBox>())
        {
            RECT rect;
            WinAPI::getListBoxItemRect(listBox->handle(), WinAPI::getListBoxIndexOf(listBox->handle(), getLabel()), rect);
            return rect.bottom - rect.top;
        }
        return 0;
    }

    ListView::ListView(Component * inParent,
                       const AttributesMapping & inAttr) :
        Super(inParent,
              inAttr,
              WC_LISTVIEW,
              WS_EX_CLIENTEDGE,
              LVS_REPORT)
    {
        if (NativeComponent * parent = NativeControl::FindNativeParent(inParent))
        {
            mEventListener.connect(parent->el(), WM_NOTIFY, boost::bind(&ListView::handleGetDispInfo, this, _1, _2));
        }
    }


    ListView::~ListView()
    {
    }


    bool ListView::init()
    {
        // Apply min widths to columns
        // XUL Hierarchy: listbox/listcols/listcol
        if (XMLListCols * listColsEl = el()->findChildOfType<XMLListCols>())
        {
            // First get the column widths
            std::vector<int> colWidths;
            std::vector<XMLListCol *> cols;
            listColsEl->getElementsByType<XMLListCol>(cols);
            for (size_t colIdx = 0; colIdx != cols.size(); ++colIdx)
            {
                colWidths.push_back(cols[colIdx]->component()->getWidth());
            }

            // Apply the column widths
            for (size_t colIdx = 0; colIdx != colWidths.size(); ++colIdx)
            {
                ListView_SetColumnWidth(handle(), colIdx, colWidths[colIdx]);
            }
        }
        return Super::init();
    }


    bool ListView::initAttributeControllers()
    {
        return Super::initAttributeControllers();
    }


    int ListView::calculateWidth(SizeConstraint inSizeConstraint) const
    {
        return 100;
    }


    int ListView::calculateHeight(SizeConstraint inSizeConstraint) const
    {
        return 100;
    }


    void ListView::onChildAdded(Component * inChild)
    {
        if (XMLListBox * listBox = el()->downcast<XMLListBox>())
        {
            if (ListItem * item = inChild->downcast<ListItem>())
            {
                WinAPI::addStringToListBox(handle(), item->getLabel());
            }
        }
    }


    void ListView::addListHeader(ListHeader * inListHeader)
    {
        WinAPI::addColumnToListView(handle(), 0, inListHeader->getLabel());
    }


    LRESULT ListView::handleGetDispInfo(WPARAM wParam, LPARAM lParam)
    {
        LPNMHDR messageHeader = (LPNMHDR)lParam;
        if (LVN_GETDISPINFO == messageHeader->code)
        {
            NMLVDISPINFO * dispInfo = (NMLVDISPINFO *)lParam;
            size_t columnIndex = dispInfo->item.iSubItem;

            std::vector<XMLListItem *> listItems;
            el()->getElementsByType<XMLListItem>(listItems);

            if (dispInfo->item.iItem >= static_cast<int>(listItems.size()))
            {
                //
                return 1;
            }

            // listbox/listitem/listcell
            std::vector<XMLListCell *> listCells;
            listItems[dispInfo->item.iItem]->getElementsByType<XMLListCell>(listCells);
            assert(columnIndex < listCells.size());
            if (columnIndex < listCells.size())
            {
                std::string label = listCells[columnIndex]->component()->downcast<ListCell>()->getLabel();
                static std::wstring utf16Label;
                utf16Label = ToUTF16(label);
                dispInfo->item.pszText = const_cast<LPWSTR>(utf16Label.c_str());
            }
        }
        return 0;
    }

} // namespace XULWin
