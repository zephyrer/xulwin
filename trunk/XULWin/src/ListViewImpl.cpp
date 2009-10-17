#include "XULWin/ListViewImpl.h"
#include "XULWin/ListBox.h"
#include "XULWin/ListCell.h"
#include "XULWin/ListCellImpl.h"
#include "XULWin/ListHeaderImpl.h"
#include "XULWin/ListItem.h"
#include "XULWin/ListItemImpl.h"
#include "XULWin/Decorator.h"
#include "XULWin/Unicode.h"
#include "XULWin/WinUtils.h"
#include <boost/bind.hpp>


namespace XULWin
{

    ListViewImpl::ListViewImpl(ElementImpl * inParent,
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


    bool ListViewImpl::initImpl()
    {
        return Super::initImpl();
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
    
    
    void ListViewImpl::onChildAdded(ElementImpl * inChild)
    {
        if (ListBox * listBox = el()->downcast<ListBox>())
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
            
            std::vector<ListItem*> listItems;
            el()->getElementsByType<ListItem>(listItems);

            if (dispInfo->item.iItem >= listItems.size())
            {
                //
                return 1;
            }

            // listbox/listitem/listcell
            std::vector<ListCell*> listCells;
            listItems[dispInfo->item.iItem]->getElementsByType<ListCell>(listCells);
            assert (columnIndex < listCells.size());
            if (columnIndex < listCells.size())
            {
                std::string label = listCells[columnIndex]->impl()->downcast<ListCellImpl>()->getLabel();
                static std::wstring utf16Label;
                utf16Label = ToUTF16(label);
                dispInfo->item.pszText = const_cast<LPWSTR>(utf16Label.c_str());
            }
        }
        return 0;
    }
    
} // namespace XULWin
