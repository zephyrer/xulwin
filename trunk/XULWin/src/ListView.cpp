#include "XULWin/ListView.h"
#include "XULWin/Decorator.h"
#include "XULWin/Element.h"
#include "XULWin/Elements.h"
#include "XULWin/ListCell.h"
#include "XULWin/ListCol.h"
#include "XULWin/ListCols.h"
#include "XULWin/ListHeader.h"
#include "XULWin/ListItem.h"
#include "XULWin/Unicode.h"
#include "XULWin/WinUtils.h"
#include <boost/bind.hpp>


namespace XULWin
{

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
