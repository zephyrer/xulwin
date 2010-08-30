#ifndef WINUTILS_H_INCLUDED
#define WINUTILS_H_INCLUDED


#include "XULWin/Node.h"
#include "XULWin/Types.h"
#include "XULWin/Windows.h"
#include <boost/function.hpp>
#include <boost/noncopyable.hpp>
#include <map>
#include <string>


namespace XULWin
{

namespace WinAPI
{

    class CurrentDirectoryChanger
    {
    public:
        CurrentDirectoryChanger(const std::string & inTargetDir);

        ~CurrentDirectoryChanger();

    private:
        CurrentDirectoryChanger(const CurrentDirectoryChanger &);
        CurrentDirectoryChanger & operator=(const CurrentDirectoryChanger &);
        TCHAR mOldDir[MAX_PATH+1];
    };


    // Initializes WinAPI Common Controls
    class CommonControlsInitializer
    {
    public:
        CommonControlsInitializer();
    };

    HWND getDefaultWindow();

    std::string getCurrentDirectory();

    std::string getEnvironmentVariable(const std::string & inVariableName);

    std::string getApplicationDirectory(HINSTANCE hInstance);

    std::string getProgramFilesDirectory();

    std::string getLastError(DWORD lastError);

    SIZE getSizeDifferenceBetweenWindowRectAndClientRect(HWND inHandle);

   
    // Opens default browser and navigates to url.
    void navigateURL(const std::string & inURL);


    //
    // Window functions
    // 
    int Window_GetWidth(HWND inHandle);

    int Window_GetHeight(HWND inHandle);

    void Window_SetWidth(HWND inHandle, int inWidth);

    void Window_SetHeight(HWND inHandle, int inHeight);


    //
    // ComboBox functions
    //
    void ComboBox_Add(HWND inHandle, const std::string & inString);

    void ComboBox_RemoveByIndex(HWND inHandle, int inIndex);

    void ComboBox_Clear(HWND inHandle);

    // returns CB_ERR (-1) if not found
    int ComboBox_FindString(HWND inHandle, const std::string & inString, int inOffset = -1);

    int ComboBox_Size(HWND inHandle);

    // Returns CB_ERR (-1) if an error occurs
    int ComboBox_ItemHeight(HWND inHandle, int inItemIndex);

    int ComboBox_GetSelectedIndex(HWND inHandle);

    void ComboBox_SetSelectedIndex(HWND inHandle, int inItemIndex);

    //
    // ListBox functions
    //
    std::string ListBox_getByIndex(HWND inHandle, int inIndex);

    void ListBox_Add(HWND inHandle, const std::string & inString);

    void ListBox_Remove(HWND inHandle, int inIndex);

    int ListBox_GetSize(HWND inHandle);

    // returns LB_ERR (-1) if not found
    int ListBox_GetIndexOf(HWND inHandle, const std::string & inStringValue);

    void ListBox_GetItemRect(HWND inHandle, int inIndex, RECT & outRect);

    // returns LB_ERR is nothing is selected
    int ListBox_GetSelectedIndex(HWND inHandle);

    // Use index -1 to deselect
    void ListBox_SetSelectedIndex(HWND inHandle, int inIndex);

    //
    // ListView functions
    //

    // Returns index of column, -1 if failed
    int ListView_AddColumn(HWND inHandle, int inIndex, const std::string & inText);


    //
    // Misc
    //
    HFONT getFont(HWND inHandle);

    SIZE getTextSize(HWND inHandle, const std::string & inText);

    SIZE getTextSize(const std::string & inText);

    //
    // Window functions (everything is a window)
    //
    std::string Window_GetText(HWND inHandle);

    void Window_SetText(HWND inHandle, const std::string & inText);
    
    void Window_AddStyle(HWND inHandle, LONG inStyle);

    LONG Window_GetStyles(HWND inHandle);

    void Window_SetStyle(HWND inHandle, LONG inStyle);

    void Window_RemoveStyle(HWND inHandle, LONG inStyle);

    bool Window_HasStyle(HWND inHandle, LONG inStyle);

    void Window_SetVisible(HWND inHandle, bool inVisible);

    bool Window_IsVisible(HWND inHandle);

    bool Window_IsDisabled(HWND inHandle);

    void Window_SetDisabled(HWND inHandle, bool inDisable);

    int Window_GetMultilineTextHeight(HWND inHandle);

    //
    // Checkbox Functions
    //
    enum CheckState
    {
        CHECKED = BST_CHECKED,
        UNCHECKED = BST_UNCHECKED,
        PARTIALLY_CHECKED = BST_INDETERMINATE
    };

    void Checkbox_(HWND inHandle, CheckState inState);

    CheckState CheckBox_GetState(HWND inHandle);

    bool CheckBox_IsChecked(HWND inHandle);

    void CheckBox_SetChecked(HWND inHandle, bool inChecked);


    //
    // ProgressMeter functions
    //
    void ProgressMeter_Init(HWND inHandle, int inLimit);

    void ProgressMeter_Advance(HWND inHandle);

    void ProgressMeter_SetProgress(HWND inHandle, int inProgress);

    int ProgressMeter_GetProgress(HWND inHandle);


    void setTextBoxReadOnly(HWND inHandle, bool inReadOnly);

    bool isTextBoxReadOnly(HWND inHandle);

    void setScrollInfo(HWND inHandle, int inTotalHeight, int inPageHeight, int inCurrentPosition);

    void getScrollInfo(HWND inHandle, int & outTotalHeight, int & outPageHeight, int & outCurrentPosition);

    int getScrollPos(HWND inHandle);

    void setScrollPos(HWND inHandle, int inPos);

    void appendTabPanel(HWND inHandle, const std::string & inTitle);

    //
    // Menu functions
    //
    void Menu_InsertItem(HMENU inMenuHandle, UINT inIndex);

    void Menu_InsertItem(HMENU inMenuHandle, UINT inIndex, UInt32 inComponentId, const std::string & inText);

    void Menu_InsertSubMenu(HMENU inMenuHandle, UINT inIndex, HMENU inSubMenu, const std::string & inText);

    struct MenuItemInfo
    {
        MenuItemInfo(int inId, const std::string & inLabel) : id(inId), label(inLabel) {}
        int id;
        std::string label;
    };
    typedef GenericNode<
    MenuItemInfo,
    ContainerPolicy_Vector,
    PointerPolicy_Shared
    > MenuNode;
    HMENU Menu_Create(const MenuNode & inMenuNode);

    void Menu_SetItemEnabled(HMENU inMenuHandle, UInt32 inComponentId, bool inEnabled);

    void Menu_SetItemChecked(HMENU inMenuHandle, UInt32 inComponentId, bool inChecked);

    typedef boost::function<void()> TimerAction;
    void setTimeout(TimerAction inAction, int inDelayInMilliseconds);

    class Timer : boost::noncopyable
    {
    public:
        Timer();

        ~Timer();

        void start(const TimerAction & inAction, int inDelayInMilliseconds);

        void stop();

    private:
        static void CALLBACK OnTimerEvent(HWND inHWND, UINT inMessage, UINT_PTR inTimerId, DWORD inTime);
        typedef std::map<Timer *, UINT_PTR> TimerMapping;
        static TimerMapping sTimerMapping;
        TimerAction mTimerAction;
        UINT_PTR mTimerId;
    };

    UInt32 Toolbar_GetToolbarButtonSize(HWND inToolbarHandle, UInt32 inToolbarButtonId);

    RECT Toolbar_GetToolbarButtonRect(HWND inToolbarHandle, UInt32 inToolbarButtonId);

    void Toolbar_SetButtonWidth(HWND inToolbarHandle, UInt32 inToolbarButtonId, int inWidth);

} // namespace WinAPI

} // namespace XULWin


#endif // WINUTILS_H_INCLUDED
