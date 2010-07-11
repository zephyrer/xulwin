#include "XULWin/WindowsListView.h"
#include "XULWin/ErrorReporter.h"
#include "XULWin/Gdiplus.h"
#include "XULWin/Unicode.h"
#include "XULWin/WinUtils.h"
#include <boost/bind.hpp>
#include <commctrl.h>


namespace XULWin
{

namespace WinAPI
{

    ListView::Parents ListView::sParents;


    ListView::ListView (HMODULE inModuleHandle, HWND inParent, int inChildId) :
        mModuleHandle(inModuleHandle),
        mParentWindow(inParent),
        mChildId(inChildId),
        mHandle(0),
        mParentProc(0)
    {
        mHandle = CreateWindow(WC_LISTVIEW, 
                               L"", 
                               WS_CHILD, 
                               0, 0, 0, 0,
                               inParent, 
                               (HMENU)mChildId, 
                               inModuleHandle, 
                               NULL);

        mParentProc = (WNDPROC)(LONG_PTR)SetWindowLongPtr(mParentWindow, GWLP_WNDPROC, (LONG)(LONG_PTR)ListView::ParentProc);
        sParents.insert(std::make_pair(this, mParentWindow));
    }


    ListView::~ListView ()
    {
        mListItems.clear();

        (WNDPROC)(LONG_PTR)SetWindowLongPtr(mParentWindow, GWLP_WNDPROC, (LONG)(LONG_PTR)mParentProc);

        Parents::iterator parentIt = sParents.find(this);
        if (parentIt != sParents.end())
        {
            sParents.erase(parentIt);
        }

        if (mHandle)
        {
            ::DeleteObject(mHandle);
            mHandle = 0;
        }
    }


    HMODULE ListView::moduleHandle() const
    {
        return mModuleHandle;
    }


    HWND ListView::handle() const
    {
        return mHandle;
    }


    void ListView::add(ListItem * inListItem)
    {
        SendMessage(handle(), LVM_INSERTITEM, 0, (LPARAM)inListItem);
        ListItemPtr listItemPtr(inListItem);
        mListItems.push_back(listItemPtr);
    }


    size_t ListView::size() const
    {
        return mListItems.size();
    }


    bool ListView::empty() const
    {
        return mListItems.empty();
    }


    void ListView::removeByIndex(size_t inIndex)
    {
        ListItems::iterator it = mListItems.begin() + inIndex;
        if (it != mListItems.end())
        {
            mListItems.erase(it);            
            if (FALSE == ::SendMessage(mHandle,
                                       LVM_DELETEITEM,
                                       LPARAM(inIndex),
                                       WPARAM(0)))
            {
                ReportError("Failed to delete item from list view. Last error: " + getLastError(::GetLastError()));
            }
        }
    }


    void ListView::clear()
    {
        while (!empty())
        {
            removeByIndex(0);
        }
    }


    const ListItem * ListView::getByIndex(size_t inIndex) const
    {
        return mListItems[inIndex].get();
    }


    ListItem * ListView::getByIndex(size_t inIndex)
    {
        return mListItems[inIndex].get();
    }


    LRESULT CALLBACK ListView::ParentProc(HWND hWnd, UINT inMessage, WPARAM wParam, LPARAM lParam)
    {
        Parents::iterator it = sParents.begin(), end = sParents.end();
        for (; it != end; ++it)
        {
            if (it->second == hWnd)
            {
                break;
            }
        }

        if (it == sParents.end())
        {
            return DefWindowProc(hWnd, inMessage, wParam, lParam);
        }

        ListView * pThis = it->first;

        switch (inMessage)
        {
            case WM_NOTIFY:
            {
                LPNMHDR notifyMsgHeader = (LPNMHDR)lParam;
                LPNMLISTVIEW notifyListViewMsg = (LPNMLISTVIEW)lParam;
                if (notifyMsgHeader->hwndFrom == pThis->handle() ||
                    notifyListViewMsg->hdr.hwndFrom == pThis->handle())
                {
                    switch (notifyMsgHeader->code)
                    {
                        case NM_CUSTOMDRAW:
                        {
                            LPNMLVCUSTOMDRAW listViewCustomDrawMsg = (LPNMLVCUSTOMDRAW)lParam;
                            LPNMCUSTOMDRAW notifyCustomDrawMsg = (LPNMCUSTOMDRAW)lParam;
                            LRESULT result = 0;
                            switch (notifyCustomDrawMsg->dwDrawStage)
                            {
                                case CDDS_PREPAINT:
                                {
                                    return CDRF_NOTIFYITEMDRAW;
                                }
                                case CDDS_ITEMPREPAINT:
                                {
                                    return CDRF_NOTIFYPOSTPAINT;
                                }
                                case CDDS_ITEMPOSTPAINT:
                                {
                                    if (ListItem * item = (ListItem*)notifyCustomDrawMsg->lItemlParam)
                                    {
                                        item->draw(listViewCustomDrawMsg->nmcd.hdc,
                                                   listViewCustomDrawMsg->nmcd.rc);
                                    }
                                    return CDRF_DODEFAULT;
                                }
                            }
                            break;
                        }
                    }
                }
                break;
            }
        }
        return CallWindowProc(pThis->mParentProc, hWnd, inMessage, wParam, lParam);
    }


    void ListItem::draw(HDC inHDC, const RECT & inRect)
    {
    }


    ListItem::ListItem(ListView * inListView) :
        mListView(inListView)
    {
        LVITEM item;
        memset(&item, 0, sizeof(item));
        item.mask = LVIF_PARAM;
        item.iItem = inListView->size();
        item.iSubItem = 0;
        if (-1 == ::SendMessage(inListView->handle(), LVM_INSERTITEM, 0, (LPARAM)&item))
        {
            ReportError("Failed to add item to list view. Last error: " + getLastError(::GetLastError()));
        }
    }


    ListItem_Text::ListItem_Text(ListView * inListView, const std::string & inText) :
        ListItem(inListView),
        mText(ToUTF16(inText))
    {        
    }


    void ListItem_Text::draw(HDC inHDC, const RECT & inRect)
    {
        RECT tmp = inRect;
        int oldBkMode = ::SetBkMode(inHDC, TRANSPARENT);
        DWORD fgColor = GetSysColor(COLOR_MENUTEXT);
        int oldTextColor = ::SetTextColor(inHDC, fgColor);
        ::DrawText(inHDC,
                   mText.c_str(),
                   mText.length(),
                   &tmp,
                   DT_LEFT | DT_VCENTER | DT_SINGLELINE);

        // Release stuff
        ::SetTextColor(inHDC, oldTextColor);
        ::SetBkMode(inHDC, oldBkMode);
    }


} // namespace WinAPI

} // namespace XULWin
