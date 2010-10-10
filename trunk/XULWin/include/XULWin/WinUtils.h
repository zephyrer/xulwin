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


    //
    // CurrentDirectoryChanger class
    //
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


    //
    // CommonControlsInitializer class
    //
    class CommonControlsInitializer
    {
    public:
        CommonControlsInitializer();
    };


    //
    // System functions
    //
    std::string System_GetCurrentDirectory();

    std::string System_GetEnvironmentVariable(const std::string & inVariableName);

    std::string System_GetEnvironmentVariable(HINSTANCE hInstance);

    std::string System_GetProgramFilesDirectory();

    // Set the thread name that will be shown in the Visual Studio debugger
    void System_SetThreadName(WORD inThreadId, const std::string & inThreadName);

    std::string System_GetLastError(DWORD lastError);
   
    // Opens default browser and navigates to url.
    void System_NavigateURL(const std::string & inURL);


    //
    // Window functions (everything is a window)
    // 
    HWND Window_GetDefaultWindow();

    int Window_GetWidth(HWND inHandle);

    int Window_GetHeight(HWND inHandle);

    void Window_SetWidth(HWND inHandle, int inWidth);

    void Window_SetHeight(HWND inHandle, int inHeight);

    std::string Window_GetText(HWND inHandle);

    void Window_SetText(HWND inHandle, const std::string & inText);

    SIZE Window_GetTextSize(HWND inHandle, const std::string & inText);

    SIZE Window_GetTextSize(const std::string & inText);

    int Window_GetMultilineTextHeight(HWND inHandle);

    SIZE Window_GetSizeDifferenceBetweenWindowRectAndClientRect(HWND inHandle);

    void Window_SetVisible(HWND inHandle, bool inVisible);

    bool Window_IsVisible(HWND inHandle);

    bool Window_IsDisabled(HWND inHandle);

    void Window_SetDisabled(HWND inHandle, bool inDisable);
    
    void Window_AddStyle(HWND inHandle, LONG inStyle);

    LONG Window_GetStyles(HWND inHandle);

    void Window_SetStyle(HWND inHandle, LONG inStyle);

    void Window_RemoveStyle(HWND inHandle, LONG inStyle);

    bool Window_HasStyle(HWND inHandle, LONG inStyle);

    HFONT Window_GetFont(HWND inHandle);


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
    // SpinButton functions
    //
    HWND SpinButton_GetBuddy(HWND inHandle);

    HWND SpinButton_SetBuddy(HWND inHandle, HWND inBuddy);

    int SpinButton_GetPos(HWND inHandle);

    int SpinButton_SetPos(HWND inHandle, int inPos);

    DWORD SpinButton_GetRange(HWND inHandle);

    void SpinButton_GetRange(HWND inHandle, int & outLower, int & outUpper);

    void SpinButton_SetRange(HWND inHandle, int inLower, int inUpper);

    void SpinButton_GetRange32(HWND inHandle, int & outLower, int & outUpper);
    
    void SpinButton_SetRange32(HWND inHandle, int inLower, int inUpper);


    //
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
    // Tab functions
    //
    void Tab_AddPanel(HWND inHandle, const std::string & inTitle);


    //
    // Checkbox Functions
    //
    enum CheckState
    {
        CHECKED = BST_CHECKED,
        UNCHECKED = BST_UNCHECKED,
        PARTIALLY_CHECKED = BST_INDETERMINATE
    };

    void Checkbox_SetState(HWND inHandle, CheckState inState);

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


    //
    // TextBox functions
    //
    void TextBox_SetReadOnly(HWND inHandle, bool inReadOnly);

    bool TextBox_IsReadOnly(HWND inHandle);


    //
    // Scrollbar functions
    //
    void Scrollbar_SetInfo(HWND inHandle, int inTotalHeight, int inPageHeight, int inCurrentPosition);

    void Scrollbar_GetInfo(HWND inHandle, int & outTotalHeight, int & outPageHeight, int & outCurrentPosition);

    int Scrollbar_GetPos(HWND inHandle);

    void Scrollbar_SetPos(HWND inHandle, int inPos);


    //
    // Menu functions
    //
    void Menu_InsertItem(HMENU inMenuHandle, UINT inIndex);

    void Menu_InsertItem(HMENU inMenuHandle, UINT inIndex, UInt32 inComponentId, const std::string & inText);

    void Menu_InsertSubMenu(HMENU inMenuHandle, UINT inIndex, HMENU inSubMenu, const std::string & inText);


    //
    // MenuItemInfo
    //
    // Contains the data fields associated with each node from the MenuNode (see below).
    //
    struct MenuItemInfo
    {
        MenuItemInfo(int inId, const std::string & inLabel) : id(inId), label(inLabel) {}
        int id;
        std::string label;
    };

    //
    // MenuNode typedef
    //
    // A menu is essentially a tree where the branches are submenus and the leafs are menu items.
    // The MenuNode class represents a full menu as a tree object.
    //
    typedef GenericNode<MenuItemInfo,
                        ContainerPolicy_Vector,
                        PointerPolicy_Shared> MenuNode;

    // Creates a menu from a tree object
    HMENU Menu_Create(const MenuNode & inMenuNode);

    void Menu_SetItemEnabled(HMENU inMenuHandle, UInt32 inComponentId, bool inEnabled);

    void Menu_SetItemChecked(HMENU inMenuHandle, UInt32 inComponentId, bool inChecked);


    //
    // Toolbar functions
    //
    UInt32 Toolbar_GetToolbarButtonSize(HWND inToolbarHandle, UInt32 inToolbarButtonId);

    RECT Toolbar_GetToolbarButtonRect(HWND inToolbarHandle, UInt32 inToolbarButtonId);

    void Toolbar_SetButtonWidth(HWND inToolbarHandle, UInt32 inToolbarButtonId, int inWidth);


    //
    // Timer class
    //
    class Timer : boost::noncopyable
    {
    public:
        Timer();

        ~Timer();

        typedef boost::function<void()> TimerAction;

        void start(const TimerAction & inAction, int inDelayInMilliseconds);

        void stop();

    private:
        static void CALLBACK OnTimerEvent(HWND inHWND, UINT inMessage, UINT_PTR inTimerId, DWORD inTime);
        typedef std::map<Timer *, UINT_PTR> TimerMapping;
        static TimerMapping sTimerMapping;
        TimerAction mTimerAction;
        UINT_PTR mTimerId;
    };

} // namespace WinAPI

} // namespace XULWin


#endif // WINUTILS_H_INCLUDED
