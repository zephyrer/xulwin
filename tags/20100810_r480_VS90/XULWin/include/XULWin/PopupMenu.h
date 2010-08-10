#ifndef POPUPMENU_H_INCLUDED
#define POPUPMENU_H_INCLUDED


#include "XULWin/Windows.h"
#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>
#include <map>
#include <string>
#include <vector>


namespace XULWin
{
    class Initializer;

    namespace WinAPI
    {

        class PopupMenu;

        class PopupMenuItem : public boost::noncopyable
        {
        public:
            // creates a separator
            PopupMenuItem();

            PopupMenuItem(int inId, const std::string & inText);

            void setAction(boost::function<void()> inAction);

            int id() const;

            const std::string & text() const;

            bool isEnabled() const;

            void setEnabled(bool inEnabled);

            bool isChecked() const;

            void setChecked(bool inChecked);

        private:
            friend class PopupMenu;
            void performAction();

            const static int SeparatorID;
            boost::function<void()> mAction;
            int mId;
            std::string mText;
            bool mEnabled;
            bool mChecked;
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

            void show(const POINT & inLocation, const RECT & inExcludeRect);

        private:
            friend class XULWin::Initializer;
            static void Register(HMODULE inModuleHandle);
            static LRESULT CALLBACK MessageHandler(HWND hWnd, UINT inMessage, WPARAM wParam, LPARAM lParam);
            // PopupMenu is key because it is garantueed to be unique, the HWND isn't
            typedef std::map<PopupMenu *, HWND> Instances;
            static Instances sInstances;

            WNDPROC mOrigProc;
            HWND mHelperWindow;
            HMENU mHandle;
            bool mOwnerDraw;
            int mSize;
            std::vector<boost::shared_ptr<PopupMenu> > mSubmenus;

            typedef std::map<int, boost::shared_ptr<PopupMenuItem> > Items;
            Items mItems;
        };


    } // namespace WinAPI

} // namespace XULWin


#endif // POPUPMENU_H_INCLUDED
