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

    namespace Windows
    {

        class CurrentDirectoryChanger
        {
        public:
            CurrentDirectoryChanger(const std::string & inTargetDir);

            ~CurrentDirectoryChanger();

        private:
            CurrentDirectoryChanger(const CurrentDirectoryChanger &);
            CurrentDirectoryChanger & operator=(const CurrentDirectoryChanger &);
            TCHAR mOldDir[MAX_PATH];
        };


        // Initializes Windows Common Controls
        class CommonControlsInitializer
        {
        public:
            CommonControlsInitializer();
        };

        std::string getCurrentDirectory();

        std::string getEnvironmentVariable(const std::string & inVariableName);

        std::string getApplicationDirectory(HINSTANCE hInstance);

        std::string getProgramFilesDirectory();

        std::string getLastError(DWORD lastError);

        SIZE getSizeDifferenceBetweenWindowRectAndClientRect(HWND inHandle);

        void addStringToComboBox(HWND inHandle, const std::string & inString);

        void removeStringFromComboBox(HWND inHandle, int inIndex);

        void addStringToListBox(HWND inHandle, const std::string & inString);

        void removeStringFromListBox(HWND inHandle, int inIndex);

        int getListBoxItemCount(HWND inHandle);

        // returns LB_ERR (-1) if not found
        int getListBoxIndexOf(HWND inHandle, const std::string & inStringValue);

        void getListBoxItemRect(HWND inHandle, int inIndex, RECT & outRect);

        // Use index -1 to deselect
        void setListItemSelected(HWND inHandle, int inIndex);

        // Returns index of column, -1 if failed
        int addColumnToListView(HWND inHandle, int inIndex, const std::string & inText);

        void clearComboBox(HWND inHandle);

        // returns CB_ERR (-1) if not found
        int findStringInComboBox(HWND inHandle, const std::string & inString, int inOffset = -1);

        int getComboBoxItemCount(HWND inHandle);

        // Returns CB_ERR (-1) if an error occurs
        int getComboBoxItemHeight(HWND inHandle, int inItemIndex);

        void selectComboBoxItem(HWND inHandle, int inItemIndex);

        int getWindowWidth(HWND inHandle);

        int getWindowHeight(HWND inHandle);

        void setWindowWidth(HWND inHandle, int inWidth);

        void setWindowHeight(HWND inHandle, int inHeight);

        HFONT getFont(HWND inHandle);

        SIZE getTextSize(HWND inHandle, const std::string & inText);

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
            typedef std::map<UINT_PTR, Timer *> TimerMapping;
            static TimerMapping sMapping;
            TimerAction mTimerAction;
            UINT_PTR mTimerId;
        };

    } // namespace Windows

} // namespace XULWin


#endif // WINUTILS_H_INCLUDED
