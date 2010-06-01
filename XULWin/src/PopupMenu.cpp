#include "XULWin/PopupMenu.h"
#include "XULWin/Unicode.h"
#include "XULWin/WinUtils.h"
#include <sstream>


namespace XULWin
{

    namespace Windows
    {

        const int PopupMenuItem::SeparatorID = 0;

        PopupMenuItem::PopupMenuItem():
            mId(SeparatorID),
            mEnabled(true),
            mChecked(false)
        {
        }


        PopupMenuItem::PopupMenuItem(int inId, const std::string & inText) :
            mId(inId),
            mText(inText),
            mEnabled(true),
            mChecked(false)
        {
        }


        void PopupMenuItem::setAction(boost::function<void()> inAction)
        {
            mAction = inAction;
        }


        void PopupMenuItem::performAction()
        {
            if (mAction)
            {
                mAction();
            }
        }


        int PopupMenuItem::id() const
        {
            return mId;
        }


        const std::string & PopupMenuItem::text() const
        {
            return mText;
        }


        bool PopupMenuItem::isEnabled() const
        {
            return mEnabled;
        }


        bool PopupMenuItem::isChecked() const
        {
            return mChecked;
        }


        void PopupMenuItem::setEnabled(bool inEnabled)
        {
            mEnabled = inEnabled;
        }


        void PopupMenuItem::setChecked(bool inChecked)
        {
            mChecked = inChecked;
        }

        void PopupMenu::Register(HMODULE inModuleHandle)
        {
            WNDCLASSEX wndClass;
            wndClass.cbSize = sizeof(wndClass);
            wndClass.style = 0;
            wndClass.lpfnWndProc = &PopupMenu::MessageHandler;
            wndClass.cbClsExtra = 0;
            wndClass.cbWndExtra = 0;
            wndClass.hInstance = inModuleHandle;
            wndClass.hIcon = 0;
            wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
            wndClass.hbrBackground = (HBRUSH)(COLOR_BTNFACE+1);
            wndClass.lpszMenuName = NULL;
            wndClass.lpszClassName = TEXT("XULWin::PopupMenuHelper");
            wndClass.hIconSm = 0;
            if (! RegisterClassEx(&wndClass))
            {
                throw std::runtime_error("Could not register XUL::PopupMenu class.");
            }
        }


        PopupMenu::Instances PopupMenu::sInstances;


        PopupMenu::PopupMenu():
            mHandle(0),
            mOwnerDraw(false),
            mSize(0),
            mHelperWindow(0),
            mOrigProc(0)
        {
            // This window only exists for event handling. It is never visible.
            mHelperWindow = CreateWindowEx(0, TEXT("XULWin::PopupMenuHelper"), TEXT("PopupMenu"), 0, 0, 0, 0, 0, 0, 0, ::GetModuleHandle(0), 0);
            if (!mHelperWindow)
            {
                throw std::runtime_error("Failed to created parent window for the popup menu");
            }

            // Make sure the Window is hidden
            ::ShowWindow(mHelperWindow, SW_HIDE);
            
            mHandle = CreatePopupMenu();
            
            mOrigProc = (WNDPROC)(LONG_PTR)::SetWindowLongPtr(mHelperWindow, GWLP_WNDPROC, (LONG)(LONG_PTR)PopupMenu::MessageHandler);
            sInstances[this] = mHelperWindow;
        }


        PopupMenu::~PopupMenu()
        {
            if (mOrigProc)
            {
                ::SetWindowLongPtr(mHelperWindow, GWLP_WNDPROC, (LONG)(LONG_PTR)mOrigProc);
                mHelperWindow = 0;
                mOrigProc = 0;
                sInstances.erase(sInstances.find(this));
            }

            if (mHandle)
            {
                DestroyMenu(mHandle);
                mHandle = 0;
            }
        }


        HMENU PopupMenu::handle() const
        {
            return mHandle;
        }


        void PopupMenu::append(PopupMenuItem * inMenuItem)
        {
            Windows::insertMenuItem(mHandle, mSize++, inMenuItem->id(), inMenuItem->text());
            Windows::setMenuItemEnabled(mHandle, inMenuItem->id(), inMenuItem->isEnabled());
            Windows::setMenuItemChecked(mHandle, inMenuItem->id(), inMenuItem->isChecked());

            Items::iterator it = mItems.find(inMenuItem->id());
            assert(it == mItems.end());
            if (it == mItems.end())
            {
                mItems.insert(std::make_pair(inMenuItem->id(), inMenuItem));
            }
        }


        int PopupMenu::size() const
        {
            return mSize;
        }


        void PopupMenu::append(const std::string & inText, PopupMenu * inSubmenu)
        {
            Windows::insertSubMenu(mHandle, mSize++, inSubmenu->handle(), inText);
            boost::shared_ptr<PopupMenu> subMenuPtr(inSubmenu);
            mSubmenus.push_back(subMenuPtr);
        }


        void PopupMenu::show(const POINT & inLocation, const RECT & inExcludeRect)
        {
            TPMPARAMS tpm;
            tpm.cbSize = sizeof(tpm);
            tpm.rcExclude = inExcludeRect;
            TrackPopupMenuEx(mHandle,
                             TPM_LEFTALIGN | TPM_LEFTBUTTON,
                             inLocation.x + 1,
                             inLocation.y + 1,
                             mHelperWindow,
                             &tpm);

            // WM_COMMAND message is not sent before TrackPopupMenuEx returns !!
        }
        
        LRESULT CALLBACK PopupMenu::MessageHandler(HWND hWnd, UINT inMessage, WPARAM wParam, LPARAM lParam)
        {
            Instances::iterator instanceIt = sInstances.begin(), end = sInstances.end();
            for (; instanceIt != end; ++instanceIt)
            {
                if (instanceIt->second == hWnd)
                {
                    break;
                }
            }

            if (instanceIt == end)
            {
                return ::DefWindowProc(hWnd, inMessage, wParam, lParam);
            }

            PopupMenu * pThis = instanceIt->first;

            switch (inMessage)
            {
                case WM_COMMAND:
                {
                    WORD id = LOWORD(wParam);
                    WORD code = HIWORD(wParam);
                    HWND sender = (HWND)lParam;
                    if (!sender)
                    {
                        Items::iterator itemIt = pThis->mItems.find(id);
                        if (itemIt != pThis->mItems.end())
                        {
                            itemIt->second->performAction();
                            return 0;
                        }
                    }
                    return 1;
                }
            }
            return ::DefWindowProc(hWnd, inMessage, wParam, lParam);
        }


    } // namespace Windows

} // namespace XULWin
