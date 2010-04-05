#include "XULWin/ListView.h"
#include "XULWin/ListBoxElement.h"
#include "XULWin/ListCellElement.h"
#include "XULWin/ListCell.h"
#include "XULWin/ListColElement.h"
#include "XULWin/ListCol.h"
#include "XULWin/ListColsElement.h"
#include "XULWin/ListCols.h"
#include "XULWin/ListHeader.h"
#include "XULWin/ListItemElement.h"
#include "XULWin/ListItem.h"
#include "XULWin/Decorator.h"
#include "XULWin/Unicode.h"
#include "XULWin/WinUtils.h"
#include <boost/bind.hpp>


namespace XULWin
{

    ListView::ListView(Component * inParent,
                       const AttributesMapping & inAttributesMapping) :
        Super(inParent,
              inAttributesMapping,
              WC_LISTVIEW,
              WS_EX_CLIENTEDGE,
              LVS_REPORT)
    {
        if (NativeComponent * parent = NativeControl::GetThisOrParent(inParent))
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
        if (ListColsElement * listColsEl = el()->findChildOfType<ListColsElement>())
        {
            // First get the column widths
            std::vector<int> colWidths;
            std::vector<ListColElement *> cols;
            listColsEl->getElementsByType<ListColElement>(cols);
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
        if (ListBoxElement * listBox = el()->downcast<ListBoxElement>())
        {
            if (ListItem * item = inChild->downcast<ListItem>())
            {
                Windows::addStringToListBox(handle(), item->getLabel());
            }
        }
    }


    void ListView::addListHeader(ListHeader * inListHeader)
    {
        Windows::addColumnToListView(handle(), 0, inListHeader->getLabel());
    }


    LRESULT ListView::handleGetDispInfo(WPARAM wParam, LPARAM lParam)
    {
        LPNMHDR messageHeader = (LPNMHDR)lParam;
        if (LVN_GETDISPINFO == messageHeader->code)
        {
            NMLVDISPINFO * dispInfo = (NMLVDISPINFO *)lParam;
            size_t columnIndex = dispInfo->item.iSubItem;

            std::vector<ListItemElement *> listItems;
            el()->getElementsByType<ListItemElement>(listItems);

            if (dispInfo->item.iItem >= static_cast<int>(listItems.size()))
            {
                //
                return 1;
            }

            // listbox/listitem/listcell
            std::vector<ListCellElement *> listCells;
            listItems[dispInfo->item.iItem]->getElementsByType<ListCellElement>(listCells);
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
