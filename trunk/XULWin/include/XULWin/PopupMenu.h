#ifndef POPUPMENU_H_INCLUDED
#define POPUPMENU_H_INCLUDED


#include "XULWin/UniqueId.h"
#include "XULWin/Windows.h"
#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>
#include <map>
#include <string>
#include <vector>


namespace XULWin
{

    namespace Windows
    {

        class PopupMenu;

        class PopupMenuItem
        {
        public:
            /**
             * Creates a separator menu item.
             */ 
            PopupMenuItem();

            /**
             * Creates a menu item with the given text.
             */
            PopupMenuItem(UInt32 inId, const std::string & inText);

            /**
             * Set the menu action.
             */
            void setAction(boost::function<void()> inAction);

            /**
             * Gets the unique menu item id.
             */
            int id() const;

            const std::string & text() const;

            bool isEnabled() const;

            void setEnabled(bool inEnabled);

            bool isChecked() const;

            void setChecked(bool inChecked);

        private:
            friend class PopupMenu;
            void performAction();

            UInt32 mId;
            std::string mText;
            boost::function<void()> mAction;
            bool mEnabled;
            bool mChecked;
            const static int sSeparatorId;
        };


        class PopupMenu : public boost::noncopyable
        {
        public:
            PopupMenu();

            virtual ~PopupMenu();

            HMENU handle() const;

            int size() const;

            // takes ownership
            virtual void append(PopupMenuItem * inMenuItem);

            // takes ownership
            void append(const std::string & inText, PopupMenu * inSubmenu);

            void show(HWND inParent, const POINT & inLocation, const RECT & inExcludeRect);

        private:
            static LRESULT CALLBACK ParentProc(HWND hWnd, UINT inMessage, WPARAM wParam, LPARAM lParam);
            // PopupMenu is key because it is garantueed to be unique, the HWND isn't
            typedef std::map<PopupMenu *, HWND> Instances;
            static Instances sInstances;

            void subclass(HWND inHandle);

            void unsubclass();

            WNDPROC mOrigProc;
            HWND mSubclassedWindow;
            HMENU mHandle;
            bool mOwnerDraw;
            int mSize;
            std::vector<boost::shared_ptr<PopupMenu> > mSubmenus;

            typedef std::map<int, boost::shared_ptr<PopupMenuItem> > Items;
            Items mItems;
        };


    } // namespace Windows

} // namespace XULWin


#endif // POPUPMENU_H_INCLUDED
