#include "XULWin/PopupMenu.h"
#include "XULWin/ErrorReporter.h"
#include "XULWin/Unicode.h"
#include "XULWin/WinUtils.h"
#include <sstream>
#include <gdiplus.h>


namespace XULWin
{

namespace Windows
{

    const int PopupMenuItem::SeparatorID = 0;


    PopupMenuItem::PopupMenuItem():
	    mId(SeparatorID),
	    mEnabled(true),
	    mChecked(false),
	    mBitmap(0)
    {
    }


    PopupMenuItem::PopupMenuItem(int inId, const std::string & inText):
	    mId(inId),
	    mText(inText),
	    mEnabled(true),
	    mChecked(false),
	    mBitmap(0)
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


    void PopupMenuItem::setHBITMAP(HBITMAP inBitmap)
    {
	    mBitmap = inBitmap;
    }


    HBITMAP PopupMenuItem::getHBITMAP() const
    {
	    return mBitmap;
    }
    				

    void PopupMenuItem::setImage(boost::shared_ptr<Gdiplus::Bitmap> inImage)
    {
	    mImage = inImage;
    }


    boost::shared_ptr<Gdiplus::Bitmap> PopupMenuItem::getImage() const
    {
	    return mImage;
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
        unsubclass();
	    DestroyMenu(mHandle);
	    mHandle = 0;
    }


    PopupMenu::operator HMENU() const
    {
	    return mHandle;
    }


    void PopupMenu::append(PopupMenuItem * inMenuItem)
    {
	    MENUITEMINFO mii;
	    memset(&mii, 0, sizeof(mii));
	    mii.cbSize = sizeof(mii);
    	
	    updateInfo(*inMenuItem, mii);
	    BOOL result = InsertMenuItem(mHandle, mSize++, TRUE, &mii);
	    ::EnableMenuItem(mHandle, inMenuItem->id(), MF_BYCOMMAND | (inMenuItem->isEnabled() ? MF_ENABLED : MF_DISABLED | MF_GRAYED));
	    ::CheckMenuItem(mHandle, inMenuItem->id(), MF_BYCOMMAND | (inMenuItem->isChecked() ? MF_CHECKED : MF_UNCHECKED));
	    if (!result)
	    {
            std::stringstream ss;
            ss << "PopupMenu::appendMenuItem failed. Reason: " << Windows::getLastError(::GetLastError());
		    ReportError(ss.str());
	    }

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
	    PopupMenuItem item(0, inText);
	    MENUITEMINFO mii;
	    memset(&mii, 0, sizeof(mii));
	    mii.cbSize = sizeof(mii);
	    mii.fMask = MIIM_SUBMENU | MIIM_STRING;
    	
	    // copy string into dwTypeData field
	    {
		    std::wstring text = ToUTF16(inText);
		    std::size_t length = text.length();
		    TCHAR * buffer = new TCHAR[length + 1];
		    for (std::size_t i = 0; i < length; i++)
		    {
			    buffer[i] = text[i];
		    }
		    buffer[length] = 0;
		    mii.dwTypeData = (LPTSTR)buffer;
	    }
    		
	    mii.cch = (UINT)inText.size();
	    mii.hSubMenu = inSubmenu->operator HMENU();

	    BOOL result = InsertMenuItem(mHandle, mSize++, TRUE, &mii);

        boost::shared_ptr<PopupMenu> subMenuPtr(inSubmenu);
	    mSubmenus.push_back(subMenuPtr);
    			
	    if (!result)
	    {
            std::stringstream ss;
            ss << "PopupMenu::appendMenuItem failed. Reason: " << Windows::getLastError(::GetLastError());
		    ReportError(ss.str());
	    }
    }


    void PopupMenu::updateInfo(const PopupMenuItem & inMenuItem, MENUITEMINFO & ioItemInfo)
    {
	    ioItemInfo.fMask = MIIM_FTYPE | MIIM_ID | MIIM_STRING;
	    ioItemInfo.fType = (inMenuItem.id() == 0) ? MFT_SEPARATOR : 0;
    	
	    // copy string into dwTypeData field
	    {
		    std::wstring text = ToUTF16(inMenuItem.text());
		    std::size_t length = text.length();
		    TCHAR * buffer = new TCHAR[length + 1];
		    for (std::size_t i = 0; i < length; i++)
		    {
			    buffer[i] = text[i];
		    }
		    buffer[length] = 0;
		    ioItemInfo.dwTypeData = (LPTSTR)buffer;
	    }
    		
	    ioItemInfo.cch = (UINT)inMenuItem.text().size();
	    ioItemInfo.wID = inMenuItem.id();
	    if (inMenuItem.getHBITMAP())
	    {
		    ioItemInfo.fMask |= MIIM_CHECKMARKS;
		    ioItemInfo.hbmpChecked = inMenuItem.getHBITMAP();
		    ioItemInfo.hbmpUnchecked = inMenuItem.getHBITMAP();
	    }
    }


    void PopupMenu::show(HWND inWindow, const POINT & inLocation, const RECT & inExcludeRegion)
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
        tpm.rcExclude = inExcludeRegion;
        TrackPopupMenuEx(mHandle,
                         TPM_LEFTALIGN | TPM_LEFTBUTTON,
                         inLocation.x,
                         inLocation.y,
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
