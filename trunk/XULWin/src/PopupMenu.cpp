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


    PopupMenuItem::PopupMenuItem(int inId, const std::string & inText):
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

    
    PopupMenu::Instances PopupMenu::sInstances;

    
    PopupMenu::PopupMenu():
	    mHandle(0),
        mOwnerDraw(false),
        mSize(0),
        mSubclassedWindow(0),
        mOrigProc(0)
    {
	    mHandle = CreatePopupMenu();
    }
    		

    PopupMenu::~PopupMenu()
    {
        if (mOrigProc)
        {
            unsubclass();
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


    void PopupMenu::show(HWND inWindow, const POINT & inLocation, const RECT & inExcludeRect)
    {
        HWND parent = inWindow;
        assert(parent);
        if (!parent)
        {
            parent = inWindow;
        }

        if (mOrigProc)
        {
            unsubclass();
        }
        subclass(parent);

        TPMPARAMS tpm;
        tpm.cbSize = sizeof(tpm);
        tpm.rcExclude = inExcludeRect;
        TrackPopupMenuEx(mHandle,
                         TPM_LEFTALIGN | TPM_LEFTBUTTON,
                         inLocation.x + 1,
                         inLocation.y + 1,
                         parent,
                         &tpm);

        // WM_COMMAND message is not sent before TrackPopupMenuEx returns !!
    }


    void PopupMenu::subclass(HWND inHandle)
    {
        assert(!mOrigProc);
        assert(!mSubclassedWindow);
        mSubclassedWindow = inHandle;
        mOrigProc = (WNDPROC)(LONG_PTR)::SetWindowLongPtr(mSubclassedWindow, GWLP_WNDPROC, (LONG)(LONG_PTR)PopupMenu::ParentProc);

        // Remember instance and handle for PopupMenu::ParentProc
        sInstances[this] = inHandle;
    }

    
    void PopupMenu::unsubclass()
    {
        assert(mOrigProc);
        assert(mSubclassedWindow);
        if (mOrigProc)
        {
            ::SetWindowLongPtr(mSubclassedWindow, GWLP_WNDPROC, (LONG)(LONG_PTR)mOrigProc);
            mSubclassedWindow = 0;
            mOrigProc = 0;
            sInstances.erase(sInstances.find(this));
        }
    }

    
    LRESULT CALLBACK PopupMenu::ParentProc(HWND hWnd, UINT inMessage, WPARAM wParam, LPARAM lParam)
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
                break;
            }
        }
        return ::CallWindowProc(pThis->mOrigProc, hWnd, inMessage, wParam, lParam);
    }


} // namespace Windows

} // namespace XULWin
