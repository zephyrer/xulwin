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

    ListViewImpl::ListViewImpl(Component * inParent,
                               const AttributesMapping & inAttributesMapping) :
        Super(inParent,
              inAttributesMapping,
              WC_LISTVIEW,
              WS_EX_CLIENTEDGE,
              LVS_REPORT)
    {
        if (NativeComponent * inNativeParent = NativeControl::GetNativeThisOrParent(inParent))
        {
            mEventListener.connect(inNativeParent->el(), WM_NOTIFY, boost::bind(&ListViewImpl::handleGetDispInfo, this, _1, _2));
        }
    }


    ListViewImpl::~ListViewImpl()
    {
    }


    bool ListViewImpl::initComponent()
    {
		// Apply min widths to columns
        // XUL Hierarchy: listbox/listcols/listcol
        if (ListColsElement * listColsEl = el()->findChildOfType<ListColsElement>())
        {
            // First get the column widths
            std::vector<int> colWidths;
            std::vector<ListColElement*> cols;
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
        return Super::initComponent();
    }

    
    bool ListViewImpl::initAttributeControllers()
    {
        return Super::initAttributeControllers();
    }
        
        
    int ListViewImpl::calculateWidth(SizeConstraint inSizeConstraint) const
    {
        return 100;
    }

    
    int ListViewImpl::calculateHeight(SizeConstraint inSizeConstraint) const
    {
        return 100;
    }
    
    
    void ListViewImpl::onChildAdded(Component * inChild)
    {
        if (ListBoxElement * listBox = el()->downcast<ListBoxElement>())
        {
            if (ListItemImpl * item = inChild->downcast<ListItemImpl>())
            {
                Windows::addStringToListBox(handle(), item->getLabel());
            }
        }
    }


    void ListViewImpl::addListHeader(ListHeaderImpl * inListHeader)
    {
        Windows::addColumnToListView(handle(), 0, inListHeader->getLabel());
    }


    LRESULT ListViewImpl::handleGetDispInfo(WPARAM wParam, LPARAM lParam)
    {
        LPNMHDR messageHeader = (LPNMHDR)lParam;
        if (LVN_GETDISPINFO == messageHeader->code)
        {
            NMLVDISPINFO * dispInfo = (NMLVDISPINFO*)lParam;    
            size_t columnIndex = dispInfo->item.iSubItem;
            
            std::vector<ListItemElement*> listItems;
            el()->getElementsByType<ListItemElement>(listItems);

            if (dispInfo->item.iItem >= static_cast<int>(listItems.size()))
            {
                //
                return 1;
            }

            // listbox/listitem/listcell
            std::vector<ListCellElement*> listCells;
            listItems[dispInfo->item.iItem]->getElementsByType<ListCellElement>(listCells);
            assert (columnIndex < listCells.size());
            if (columnIndex < listCells.size())
            {
                std::string label = listCells[columnIndex]->component()->downcast<ListCellImpl>()->getLabel();
                static std::wstring utf16Label;
                utf16Label = ToUTF16(label);
                dispInfo->item.pszText = const_cast<LPWSTR>(utf16Label.c_str());
            }
        }
        return 0;
    }
    
} // namespace XULWin
