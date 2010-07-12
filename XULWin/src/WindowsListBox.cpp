#include "XULWin/WindowsListBox.h"
#include "XULWin/ErrorReporter.h"
#include "XULWin/WinUtils.h"
#include <commctrl.h>


namespace XULWin
{

namespace WinAPI
{


    ListBox::ListBox(HMODULE inModuleHandle, HWND inParent, int inChildId) :
        mModuleHandle(inModuleHandle),
        mChildId(inChildId),
        mHandle(0)
    {
        mHandle = CreateWindowEx(WS_EX_CLIENTEDGE,
                                 TEXT("LISTBOX"),
                                 L"", 
                                 WS_CHILD | WS_CHILD | WS_CLIPCHILDREN | WS_VISIBLE,
                                 0, 0, 0, 0,
                                 inParent, 
                                 (HMENU)mChildId, 
                                 inModuleHandle, 
                                 NULL);

        // set modern font
        ::SendMessage(mHandle, WM_SETFONT, (WPARAM)::GetStockObject(DEFAULT_GUI_FONT), MAKELPARAM(FALSE, 0));
    }


    ListBox::~ListBox ()
    {
        if (mHandle)
        {
            ::DeleteObject(mHandle);
            mHandle = 0;
        }
    }


    HMODULE ListBox::moduleHandle() const
    {
        return mModuleHandle;
    }


    HWND ListBox::handle() const
    {
        return mHandle;
    }


    void ListBox::add(const std::string & inText)
    {
        WinAPI::ListBox_Add(handle(), inText);
    }


    size_t ListBox::size() const
    {
        return WinAPI::ListBox_GetSize(handle());
    }


    bool ListBox::empty() const
    {
        return size() == 0;
    }


    int ListBox::getSelectedIndex() const
    {
        return WinAPI::ListBox_GetSelectedIndex(handle());
    }


    std::string ListBox::getByIndex(size_t inIndex) const
    {
        return WinAPI::ListBox_getByIndex(handle(), inIndex);
    }


    void ListBox::removeByIndex(size_t inIndex)
    {
        WinAPI::ListBox_Remove(handle(), inIndex);
    }


    void ListBox::clear()
    {        
        while (!empty())
        {
            removeByIndex(0);
        }
    }


} // namespace WinAPI

} // namespace XULWin
