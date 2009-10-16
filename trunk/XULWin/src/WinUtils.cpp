#include "XULWin/WinUtils.h"
#include "XULWin/ErrorReporter.h"
#include "XULWin/Unicode.h"
#include <boost/lexical_cast.hpp>
#include <map>
#include <commctrl.h>


namespace XULWin
{

namespace Windows
{

    CommonControlsInitializer::CommonControlsInitializer()
    {
		INITCOMMONCONTROLSEX icex;
		icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
		icex.dwICC  = ICC_BAR_CLASSES | ICC_COOL_CLASSES | ICC_STANDARD_CLASSES | ICC_WIN95_CLASSES;
		if (TRUE != InitCommonControlsEx(&icex))
        {
            throw std::runtime_error("Failed to initialized the Common Controls library.");
        }
    }
    

    CurrentDirectoryChanger::CurrentDirectoryChanger(const std::string & inTargetDir)
	{
        ::GetCurrentDirectoryW(MAX_PATH, mOldDir);
        std::wstring newDir = ToUTF16(inTargetDir);
        ::SetCurrentDirectoryW(newDir.c_str());
	}


	CurrentDirectoryChanger::~CurrentDirectoryChanger()
	{
		::SetCurrentDirectoryW(mOldDir);
	}


    std::string getCurrentDirectory()
    {
        TCHAR buffer[MAX_PATH];
        ::GetCurrentDirectory(sizeof(buffer), &buffer[0]);
        return std::string(ToUTF8(buffer)) + "/";
    }
    
    
    SIZE getSizeDifferenceBetweenWindowRectAndClientRect(HWND inHandle)
    {
	    RECT rc;
	    GetClientRect(inHandle, &rc);
    	
	    RECT rw;
	    GetWindowRect(inHandle, &rw);
    	
	    int rc_w = rc.right-rc.left;
	    int rc_h = rc.bottom-rc.top;
	    int rw_w = rw.right-rw.left;
	    int rw_h = rw.bottom-rw.top;
    	
	    SIZE theDifference;
	    theDifference.cx = rw_w-rc_w;
	    theDifference.cy = rw_h-rc_h;
    	
	    return theDifference;
    }


    void addStringToComboBox(HWND inHandle, const std::string & inString)
    {
        std::wstring utf16String = ToUTF16(inString);
        if (CB_ERR == ::SendMessage(inHandle, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)utf16String.c_str()))
        {
            ReportError(getLastError(::GetLastError()));
        }
    }

    
    void removeStringFromComboBox(HWND inHandle, int inIndex)
    {
        if (CB_ERR == ::SendMessage(inHandle, CB_DELETESTRING, (WPARAM)inIndex, (LPARAM)0))
        {
            ReportError(getLastError(::GetLastError()));
        }
    }


    void addStringToListBox(HWND inHandle, const std::string & inString)
    {
        std::wstring utf16String = ToUTF16(inString);
        if (LB_ERR == ::SendMessage(inHandle, (UINT)LB_ADDSTRING, (WPARAM)0, (LPARAM)utf16String.c_str()))
        {
            ReportError(getLastError(::GetLastError()));
        }
    }

    
    void removeStringFromListBox(HWND inHandle, int inIndex)
    {
        if (LB_ERR == ::SendMessage(inHandle, LB_DELETESTRING, (WPARAM)inIndex, (LPARAM)0))
        {
            ReportError(getLastError(::GetLastError()));
        }
    }

    
    int getListBoxItemCount(HWND inHandle)
    {
        int result = ::SendMessage(inHandle, LB_GETCOUNT, 0, 0);
        if (LB_ERR == result)
        {
            ReportError(getLastError(::GetLastError()));
        }
        return result;
    }

    
    int getListBoxIndexOf(HWND inHandle, const std::string & inStringValue)
    {
        std::wstring utf16String = ToUTF16(inStringValue);
        return (int)::SendMessage(inHandle, LB_FINDSTRINGEXACT, (WPARAM)-1, (LPARAM)utf16String.c_str());
    }

    
    void getListBoxItemRect(HWND inHandle, int inIndex, RECT & outRect)
    {
        if (LB_ERR == ::SendMessage(inHandle, LB_GETITEMRECT, (WPARAM)inIndex, (LPARAM)&outRect))
        {
            ReportError(getLastError(::GetLastError()));
        }
    }

    
    void setListItemSelected(HWND inHandle, int inIndex)
    {
        if (LB_ERR == ::SendMessage(inHandle, LB_SETCURSEL, (WPARAM)inIndex, (LPARAM)0))
        {
            // If an error occurs, the return value is LB_ERR. If the wParam
            // parameter is �1, the return value is LB_ERR even though no error
            // occurred.
            if (inIndex != -1)
            {
                ReportError(getLastError(::GetLastError()));
            }
        }
    }


    void clearComboBox(HWND inHandle)
    {
        // This message always returns CB_OKAY.
        ::SendMessage(inHandle, CB_RESETCONTENT, (WPARAM)0, (LPARAM)0);
    }


    int findStringInComboBox(HWND inHandle, const std::string & inString, int inOffset)
    {
        std::wstring utf16String = ToUTF16(inString);
        return (int)::SendMessage(inHandle, CB_FINDSTRING, (WPARAM)inOffset, (LPARAM)(LPTSTR)utf16String.c_str());
    }


    int getComboBoxItemCount(HWND inHandle)
    {
        int result = ::SendMessage(inHandle, CB_GETCOUNT, 0, 0);
        if (CB_ERR == result)
        {
            ReportError(getLastError(::GetLastError()));
        }
        return result;
    }


    int getWindowWidth(HWND inHandle)
    {
        RECT rw;
        ::GetWindowRect(inHandle, &rw);
        return rw.right - rw.left;
    }


    int getWindowHeight(HWND inHandle)
    {
        RECT rw;
        ::GetWindowRect(inHandle, &rw);
        return rw.bottom - rw.top;
    }


    void selectComboBoxItem(HWND inHandle, int inItemIndex)
    {
        if (CB_ERR == ::SendMessage(inHandle, (UINT)CB_SETCURSEL, (WPARAM)inItemIndex, (LPARAM)0))
        {
            ReportError(getLastError(::GetLastError()));
        }
    }
    
    
    int getComboBoxItemHeight(HWND inHandle, int inItemIndex)
    {
        return (int)::SendMessage(inHandle, (UINT)CB_GETITEMHEIGHT, (WPARAM)inItemIndex, (LPARAM)0);
    }


    void setWindowWidth(HWND inHandle, int inWidth)
    {
        RECT rw;
        ::GetWindowRect(inHandle, &rw);
        
        int oldWidth = rw.right - rw.left;
        int x = rw.left - (inWidth - oldWidth)/2;
        if (inWidth < oldWidth)
        {
            x = rw.left + (inWidth - oldWidth)/2;
        }
        if (0 == ::MoveWindow(inHandle, x, rw.top, inWidth, rw.bottom - rw.top, FALSE))
        {
            ReportError(getLastError(::GetLastError()));
        }
    }


    void setWindowHeight(HWND inHandle, int inHeight)
    {
        RECT rw;
        ::GetWindowRect(inHandle, &rw);
        
        int oldHeight = rw.bottom - rw.top;
        int y = rw.top - (inHeight - oldHeight)/2;
        if (inHeight < oldHeight)
        {
            y = rw.top + (inHeight - oldHeight)/2;
        }
        if (0 == ::MoveWindow(inHandle, rw.left, y, rw.right - rw.left, inHeight, FALSE))
        {
            ReportError(getLastError(::GetLastError()));
        }
    }

    
    HFONT getFont(HWND inHandle)
    {
	    return (HFONT)SendMessage(inHandle, WM_GETFONT, 0, 0);
    }
    
    
    SIZE getTextSize(HWND inHandle, const std::string & inText)
    {
	    // get the size in pixels for the given text and font
        SIZE result = {0, 0};    	
	    HDC hDC = GetDC(inHandle);
	    SelectObject(hDC, getFont(inHandle));
        std::wstring utf16Text(ToUTF16(inText));
        ::GetTextExtentPoint32(hDC, utf16Text.c_str(), (int)utf16Text.size(), &result);
	    ReleaseDC(inHandle, hDC);
	    return result;
    }


    std::string getWindowText(HWND inHandle)
    {
		std::string result;
		int length = ::GetWindowTextLength(inHandle);
		if (length > 0)
		{
			TCHAR * buffer = new TCHAR[length+1];
			::GetWindowText(inHandle, buffer, length+1);
			result = ToUTF8(buffer);
			delete [] buffer;
		}
		return result;
    }

    
    void setWindowText(HWND inHandle, const std::string & inText)
    {
        std::wstring utf16String = ToUTF16(inText);
        if (0 == ::SetWindowText(inHandle, utf16String.c_str()))
        {
            ReportError(getLastError(::GetLastError()));
        }
    }


    std::string getLastError(DWORD lastError)
    {
	    LPVOID lpMsgBuf;
	    ::FormatMessage
	    (
		    FORMAT_MESSAGE_ALLOCATE_BUFFER
		    | FORMAT_MESSAGE_FROM_SYSTEM
		    | FORMAT_MESSAGE_IGNORE_INSERTS,
		    NULL,
		    lastError,
		    MAKELANGID(LANG_NEUTRAL,SUBLANG_DEFAULT),
		    (LPTSTR)&lpMsgBuf,
		    0,
		    NULL
	    );
	    if (lpMsgBuf)
	    {
            std::wstring errorString = (LPTSTR)lpMsgBuf;
		    LocalFree(lpMsgBuf);
		    if (errorString.empty())
		    {
			    try
			    {
                    errorString = boost::lexical_cast<std::wstring>(lastError);
			    }
			    catch(...)
			    {
				    ReportError("Bad cast!");
			    }
		    }
		    return ToUTF8(errorString);
	    }
	    else
	    {
		    return "";
	    }
    }

    
    bool isWindowDisabled(HWND inHandle)
    {
        return !::IsWindowEnabled(inHandle);
    }


    void disableWindow(HWND inHandle, bool inDisable)
    {
        ::EnableWindow(inHandle, inDisable ? FALSE : TRUE);
    }


    int getMultilineTextHeight(HWND inHandle)
    {   
        int result = 0;

        HDC deviceContext(::GetDC(inHandle));

        HFONT font = (HFONT)::SendMessage(inHandle, WM_GETFONT, 0, 0);
		if (font)
        {
			::SelectObject(deviceContext, font);
        }
        
        RECT rc;
        ::GetClientRect(inHandle, &rc);

        RECT bounds;
		bounds.left = 0;
		bounds.top = 0;
		bounds.right = rc.right - rc.left;
		bounds.bottom = INT_MAX;

        std::wstring textUTF16 = ToUTF16(getWindowText(inHandle));
        
		result = ::DrawText(deviceContext, textUTF16.c_str(), (int)textUTF16.size(), &bounds, DT_CALCRECT | DT_WORDBREAK | DT_EDITCONTROL);

        ::ReleaseDC(inHandle, deviceContext);

        return result;
    }


    void setCheckBoxState(HWND inHandle, CheckState inState)
    {
        // This message always returns zero.
        ::SendMessage(inHandle, BM_SETCHECK, (WPARAM)inState, 0);
    }

    
    CheckState getCheckBoxState(HWND inHandle)
    {
        return static_cast<CheckState>(::SendMessage(inHandle, BM_GETCHECK, 0, 0));
    }


    bool isCheckBoxChecked(HWND inHandle)
    {
        return getCheckBoxState(inHandle) != UNCHECKED;
    }

    
    void setCheckBoxChecked(HWND inHandle, bool inChecked)
    {
        setCheckBoxState(inHandle, inChecked ? CHECKED : UNCHECKED);
    }

    
    void initializeProgressMeter(HWND inHandle, int inLimit)
    {
        ::SendMessage(inHandle, PBM_SETRANGE, 0, MAKELPARAM(0, inLimit));	
        ::SendMessage(inHandle, PBM_SETSTEP, 1, 0);
        ::SendMessage(inHandle, PBM_SETPOS, 0, 0);
    }


    void advanceProgressMeter(HWND inHandle)
	{
        // When the position exceeds the maximum range value, this message
        // resets the current position so that the progress indicator starts
        // over again from the beginning.
        ::SendMessage(inHandle, PBM_STEPIT, 0, 0);
	}
		

	void setProgressMeterProgress(HWND inHandle, int inProgress)
	{
        // Returns the previous position.
        ::SendMessage(inHandle, PBM_SETPOS, (WPARAM)inProgress, (LPARAM)0);
	}


    int getProgressMeterProgress(HWND inHandle)
    {
        // Returns a UINT value that represents the current position of the
        // progress bar.
        return (int)::SendMessage(inHandle, PBM_GETPOS, (WPARAM)0, (LPARAM)0);
    }


    void addWindowStyle(HWND inHandle, LONG inStyle) 
    {
        if (0 == ::SetWindowLong(inHandle, GWL_STYLE, ::GetWindowLong(inHandle, GWL_STYLE) | inStyle))
        {            
            ReportError(getLastError(::GetLastError()));
        }
    }


    LONG getWindowStyles(HWND inHandle)
    {
        return ::GetWindowLong(inHandle, GWL_STYLE);
    }


    void setWindowStyle(HWND inHandle, LONG inStyle) 
    {
        if (0 == ::SetWindowLong(inHandle, GWL_STYLE, inStyle))
        {
            ReportError(getLastError(::GetLastError()));
        }
    }


    void removeWindowStyle(HWND inHandle, LONG inStyle)
    {
        if (0 == ::SetWindowLong(inHandle, GWL_STYLE, ::GetWindowLong(inHandle, GWL_STYLE) & ~inStyle))
        {
            ReportError(getLastError(::GetLastError()));
        }
    }


    bool hasWindowStyle(HWND inHandle, LONG inStyle)
    {
        return (::GetWindowLong(inHandle, GWL_STYLE) & inStyle) != 0;
    }


    void setWindowVisible(HWND inHandle, bool inVisible)
    {
        // If the window was previously visible, the return value is nonzero.
        // If the window was previously hidden, the return value is zero.
        ::ShowWindow(inHandle, inVisible ? SW_SHOW : SW_HIDE);
    }

    
    bool isWindowVisible(HWND inHandle)
    {
        return TRUE == ::IsWindowVisible(inHandle);
    }


    void setTextBoxReadOnly(HWND inHandle, bool inReadOnly)
    {
        if (0 != ::SendMessage(inHandle, EM_SETREADONLY, inReadOnly ? TRUE : FALSE, 0))
        {
            ReportError(getLastError(::GetLastError()));
        }
    }


    bool isTextBoxReadOnly(HWND inHandle)
    {
        return hasWindowStyle(inHandle, ES_READONLY);
    }


    void setScrollInfo(HWND inHandle, int inTotalHeight, int inPageHeight, int inCurrentPosition)
    {
        SCROLLINFO si;
		si.cbSize = sizeof(SCROLLINFO);
		si.fMask = SIF_RANGE | SIF_PAGE | SIF_POS;
		si.nMin = 0;
		si.nMax = inTotalHeight;
		si.nPage = inPageHeight;
		si.nPos = inCurrentPosition;
		si.nTrackPos = 0; // is ignored by SetScrollInfo

        // The return value is the current position of the scroll box.
        ::SetScrollInfo(inHandle, SB_CTL, &si, TRUE);
    }


    void getScrollInfo(HWND inHandle, int & outTotalHeight, int & outPageHeight, int & outCurrentPosition)
    {
        SCROLLINFO si;
        ::ZeroMemory(&si, sizeof(si));
		si.cbSize = sizeof(SCROLLINFO);
		si.fMask = SIF_RANGE | SIF_PAGE | SIF_POS;

        // If the function does not retrieve any values, the return value is
        // zero. To get extended error information, call GetLastError.
        if (0 != ::GetScrollInfo(inHandle, SB_CTL, &si))
        {
            outTotalHeight = si.nMax;
            outPageHeight = si.nPage;
            outCurrentPosition = si.nPos;
        }
        else
        {
            ReportError(getLastError(::GetLastError()));
        }
    }


    int getScrollPos(HWND inHandle)
    {
        return (int)::GetScrollPos(inHandle, SB_CTL);
    }


    void setScrollPos(HWND inHandle, int inPos)
    {
        if (0 == ::SetScrollPos(inHandle, SB_CTL, inPos, TRUE))
        {
            ReportError(getLastError(::GetLastError()));
        }
    }


    void appendTabPanel(HWND inHandle, const std::string & inTitle)
    {
        TCITEM tabItem;
        tabItem.mask = TCIF_TEXT | TCIF_IMAGE; 
        tabItem.iImage = -1; 
        std::wstring text = ToUTF16(inTitle);
        tabItem.pszText = const_cast<LPWSTR>(text.c_str());
        if (-1 == TabCtrl_InsertItem(inHandle, TabCtrl_GetItemCount(inHandle), &tabItem)) 
        { 
            ReportError(getLastError(::GetLastError()));
        }
    }


    void insertMenuItem(HMENU inMenuHandle, UINT inIndex, int inCommandId, const std::string & inText)
    {        
	    MENUITEMINFO mii;
	    memset(&mii, 0, sizeof(mii));
	    mii.cbSize = sizeof(mii);	    
	    mii.fMask = MIIM_FTYPE | MIIM_ID | MIIM_STRING;
	    mii.fType = 0;

	    std::wstring itemText = ToUTF16(inText);
        std::vector<TCHAR> buffer;
        buffer.resize(itemText.length() + 1);
	    for (std::size_t i = 0; i < itemText.length(); i++)
	    {
		    buffer[i] = itemText[i];
	    }
	    buffer[itemText.length()] = 0;
	    mii.dwTypeData = (LPTSTR)&buffer[0];    		
	    mii.cch = (UINT)itemText.size();
	    mii.wID = inCommandId;
        if (0 == ::InsertMenuItem(inMenuHandle, inIndex, TRUE, &mii))
        {
            ReportError(getLastError(::GetLastError()));
        }
    }
    

    void insertSubMenu(HMENU inMenuHandle, UINT inIndex, HMENU inSubMenu, const std::string & inText)
    {
	    MENUITEMINFO mii;
	    memset(&mii, 0, sizeof(mii));
	    mii.cbSize = sizeof(mii);
	    mii.fMask = MIIM_SUBMENU | MIIM_STRING;
    	
	    std::wstring text = ToUTF16(inText);
        std::vector<TCHAR> buffer;
        buffer.resize(text.length() + 1);
        for (std::size_t i = 0; i < text.length(); i++)
	    {
		    buffer[i] = text[i];
	    }
	    buffer[text.length()] = 0;
	    mii.dwTypeData = (LPTSTR)&buffer[0];    		
	    mii.cch = (UINT)inText.size();
	    mii.hSubMenu = inSubMenu;

        if (0 == ::InsertMenuItem(inMenuHandle, inIndex, TRUE, &mii))
        {
            ReportError(getLastError(::GetLastError()));
        }
    }
    

    void setMenuItemEnabled(HMENU inMenuHandle, int inCommandId, bool inEnabled)
    {
        if (0 == ::EnableMenuItem(inMenuHandle, inCommandId, MF_BYCOMMAND | inEnabled ? MF_ENABLED : MF_DISABLED | MF_GRAYED))
        {
            ReportError(getLastError(::GetLastError()));
        }
    }
    

    void setMenuItemChecked(HMENU inMenuHandle, int inCommandId, bool inChecked)
    {
        if (0 == ::CheckMenuItem(inMenuHandle, inCommandId, MF_BYCOMMAND | inChecked ? MF_CHECKED : MF_UNCHECKED))
        {
            ReportError(getLastError(::GetLastError()));
        }
    }


    typedef std::map<UINT_PTR, TimerAction> Timers;

    static Timers sTimers;
	
    static void CALLBACK TimerCallback(HWND inHWND, UINT inMessage, UINT_PTR inTimerId, DWORD inTime)
	{
        Timers::iterator it = sTimers.find(inTimerId);
        assert (it != sTimers.end());
        if (it != sTimers.end())
        {
            ::KillTimer(NULL, inTimerId);
            it->second();
            sTimers.erase(it);
        }
	}


    void setTimeout(TimerAction inAction, int inDelayInMilliseconds)
    {
        UINT_PTR timerId = ::SetTimer(NULL, NULL, inDelayInMilliseconds, &Windows::TimerCallback);
        if (timerId != 0)
        {
            assert (sTimers.find(timerId) == sTimers.end());
            sTimers.insert(std::make_pair(timerId, inAction));
        }
        else
        {
            ReportError(getLastError(::GetLastError()));
        }
    }


} // namespace Windows

} // namespace XULWin
