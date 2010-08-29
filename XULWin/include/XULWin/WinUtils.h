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

    HFONT getFont(HWND inHandle);

    SIZE getTextSize(HWND inHandle, const std::string & inText);

    SIZE getTextSize(const std::string & inText);

    std::string getWindowText(HWND inHandle);

    void setWindowText(HWND inHandle, const std::string & inText);

    bool isWindowDisabled(HWND inHandle);

    void disableWindow(HWND inHandle, bool inDisable);

    int getMultilineTextHeight(HWND inHandle);

    enum CheckState
    {
        CHECKED = BST_CHECKED,
        UNCHECKED = BST_UNCHECKED,
        PARTIALLY_CHECKED = BST_INDETERMINATE
    };

    void setCheckBoxState(HWND inHandle, CheckState inState);

    CheckState getCheckBoxState(HWND inHandle);

    bool isCheckBoxChecked(HWND inHandle);

    void setCheckBoxChecked(HWND inHandle, bool inChecked);

    void initializeProgressMeter(HWND inHandle, int inLimit);

    void advanceProgressMeter(HWND inHandle);

    void setProgressMeterProgress(HWND inHandle, int inProgress);

    int getProgressMeterProgress(HWND inHandle);

    void addWindowStyle(HWND inHandle, LONG inStyle);

    LONG getWindowStyles(HWND inHandle);

    void setWindowStyle(HWND inHandle, LONG inStyle);

    void removeWindowStyle(HWND inHandle, LONG inStyle);

    bool hasWindowStyle(HWND inHandle, LONG inStyle);

    void setWindowVisible(HWND inHandle, bool inVisible);

    bool isWindowVisible(HWND inHandle);

    void setTextBoxReadOnly(HWND inHandle, bool inReadOnly);

    bool isTextBoxReadOnly(HWND inHandle);

    void setScrollInfo(HWND inHandle, int inTotalHeight, int inPageHeight, int inCurrentPosition);

    void getScrollInfo(HWND inHandle, int & outTotalHeight, int & outPageHeight, int & outCurrentPosition);

    int getScrollPos(HWND inHandle);

    void setScrollPos(HWND inHandle, int inPos);

    void appendTabPanel(HWND inHandle, const std::string & inTitle);

    void insertMenuSeparator(HMENU inMenuHandle, UINT inIndex);

    void insertMenuItem(HMENU inMenuHandle, UINT inIndex, UInt32 inComponentId, const std::string & inText);

    void insertSubMenu(HMENU inMenuHandle, UINT inIndex, HMENU inSubMenu, const std::string & inText);

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
    HMENU createMenu(const MenuNode & inMenuNode);

    void setMenuItemEnabled(HMENU inMenuHandle, UInt32 inComponentId, bool inEnabled);

    void setMenuItemChecked(HMENU inMenuHandle, UInt32 inComponentId, bool inChecked);

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
