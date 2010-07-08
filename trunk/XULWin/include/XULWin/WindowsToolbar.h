#ifndef WINDOWSTOOLBAR_H_INCLUDED
#define WINDOWSTOOLBAR_H_INCLUDED


#include "XULWin/Types.h"
#include "XULWin/Windows.h"
#include <boost/shared_ptr.hpp>
#include <map>
#include <string>
#include <vector>


namespace XULWin
{

    namespace WinAPI
    {

        class AbstractToolbarItem;
        class ToolbarDropDown;
        class ToolbarSpring;

        class WindowsToolbar
        {
        public:
            class EventHandler
            {
            public:
                virtual void onRequestFocus() = 0;
            };
            typedef std::vector<boost::shared_ptr<AbstractToolbarItem> > ToolbarItems;

            typedef std::map<HWND, RECT> CustomWindowPositions;

            WindowsToolbar (EventHandler * inEventHandler, HMODULE inModuleHandle, HWND inParentWindow, int inID);

            ~WindowsToolbar ();


            void add(AbstractToolbarItem * inToolbarItem);

            size_t size() const;

            bool empty() const;

            void remove(size_t inIndex);

            void clear();

            void enable(size_t inIndex);

            void disable(size_t inIndex);

            const AbstractToolbarItem * get(size_t inIndex) const;

            AbstractToolbarItem * get(size_t inIndex);

            const AbstractToolbarItem * getToolbarItemByCommandId(UInt32 inComponentId) const;

            AbstractToolbarItem * getToolbarItemByCommandId(UInt32 inComponentId);


            HMODULE moduleHandle() const;

            HWND handle() const;

            bool hasFocus() const;

            void setFocus();

            void rebuildLayout();

            void buildToolbar();

        private:
            friend class ToolbarDropDown;

            static LRESULT CALLBACK ToolbarProc(HWND hWnd, UINT inMessage, WPARAM wParam, LPARAM lParam);

            static LRESULT CALLBACK ParentProc(HWND hWnd, UINT inMessage, WPARAM wParam, LPARAM lParam);

            static void buildToolbar(HWND inToolbarHandle, HFONT hFont, const ToolbarItems & inToolbarItems, CustomWindowPositions & outCustomWindowPositions);

            static void updateToolbarButtonSizes(HWND inToolbarHandle, HFONT hFont, const ToolbarItems & inToolbarItems);

            static void applySpring(HWND inToolbarHandle, const ToolbarItems & inToolbarItems, int inSpringID);

            static ToolbarItems::const_iterator findByCommandID(const ToolbarItems & inToolbarItems, UInt32 inComponentId);

            void setActiveDropDownToNull();

            EventHandler * mEventHandler;

            HMODULE mModuleHandle;
            HWND mParentWindow;
            HWND mHandle;
            HFONT mFont;
            int mID;
            ToolbarDropDown * mActiveDropDown;

            WNDPROC mParentProc;
            WNDPROC mToolbarProc;

            typedef std::map<WindowsToolbar *, HWND> ParentMapping;
            static ParentMapping sInstancesParent;

            typedef std::map<WindowsToolbar *, HWND> InstanceMapping;
            static InstanceMapping sInstances;

            ToolbarItems mToolbarItems;
            CustomWindowPositions mCustomWindowPositions;
        };

    } // namespace WinAPI

} // namespace XULWin


#endif // WINDOWSTOOLBAR_H_INCLUDED
