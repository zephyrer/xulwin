#include "XULWin/NativeComponent.h"
#include "XULWin/EventListener.h"
#include "XULWin/Menu.h"
#include "XULWin/WinUtils.h"


namespace XULWin
{

    NativeComponent::ComponentsByHandle NativeComponent::sComponentsByHandle;
    NativeComponent::ComponentsById NativeComponent::sComponentsById;
    HMODULE NativeComponent::sModuleHandle(0);

    NativeComponent::NativeComponent(Component * inParent, const AttributesMapping & inAttr) :
        ConcreteComponent(inParent),
        mHandle(0),
        mModuleHandle(NativeComponent::GetModuleHandle()),
        mOrigProc(0),
        mOwnsHandle(true)
    {
    }


    NativeComponent::~NativeComponent()
    {
        if (mHandle && mOwnsHandle)
        {
            unregisterHandle();
            unsubclass();
            ::DestroyWindow(mHandle);
        }
    }


    bool NativeComponent::init()
    {
        return Super::init();
    }


    void NativeComponent::subclass()
    {
        assert(!mOrigProc);
        mOrigProc = (WNDPROC)(LONG_PTR)::SetWindowLongPtr(mHandle, GWL_WNDPROC, (LONG)(LONG_PTR)&NativeComponent::MessageHandler);
    }


    void NativeComponent::unsubclass()
    {
        if (mOrigProc)
        {
            ::SetWindowLongPtr(mHandle, GWL_WNDPROC, (LONG)(LONG_PTR)mOrigProc);
            mOrigProc = 0;
        }
    }


    void NativeComponent::registerHandle()
    {
        assert(sComponentsByHandle.find(mHandle) == sComponentsByHandle.end());
        sComponentsByHandle.insert(std::make_pair(mHandle, this));

        assert(sComponentsById.find(mComponentId.value()) == sComponentsById.end());
        sComponentsById.insert(std::make_pair(mComponentId.value(), this));
    }


    void NativeComponent::unregisterHandle()
    {
        ComponentsById::iterator itById =sComponentsById.find(mComponentId.value());
        assert(itById !=sComponentsById.end());
        if (itById != sComponentsById.end())
        {
            sComponentsById.erase(itById);
        }

        ComponentsByHandle::iterator itByHandle = sComponentsByHandle.find(mHandle);
        assert(itByHandle != sComponentsByHandle.end());
        if (itByHandle != sComponentsByHandle.end())
        {
            sComponentsByHandle.erase(itByHandle);
        }
    }


    void NativeComponent::setHandle(HWND inHandle, bool inPassOwnership)
    {
        mHandle = inHandle;
        mOwnsHandle = inPassOwnership;
    }


    NativeComponent * NativeComponent::FindByHandle(HWND inHandle)
    {
        ComponentsByHandle::iterator it = sComponentsByHandle.find(inHandle);
        if (it != sComponentsByHandle.end())
        {
            return it->second;
        }
        return 0;
    }


    NativeComponent * NativeComponent::FindById(int inId)
    {
        ComponentsById::iterator it = sComponentsById.find(inId);
        if (it !=sComponentsById.end())
        {
            return it->second;
        }
        return 0;
    }


    void NativeComponent::invalidateRect() const
    {
        ::RedrawWindow(handle(), NULL, NULL, RDW_INVALIDATE | RDW_ALLCHILDREN);
        ::InvalidateRect(handle(), NULL, TRUE);
    }


    bool NativeComponent::isDisabled() const
    {
        return WinAPI::isWindowDisabled(handle());
    }


    void NativeComponent::setDisabled(bool inDisabled)
    {
        WinAPI::disableWindow(handle(), inDisabled);
    }


    std::string NativeComponent::getLabel() const
    {
        return WinAPI::getWindowText(handle());
    }


    void NativeComponent::setLabel(const std::string & inLabel)
    {
        WinAPI::setWindowText(handle(), inLabel);
    }


    void NativeComponent::setHidden(bool inHidden)
    {
        Super::setHidden(inHidden);
        WinAPI::setWindowVisible(handle(), !inHidden);
    }


    void NativeComponent::SetModuleHandle(HMODULE inModule)
    {
        sModuleHandle = inModule;
    }


    HMODULE NativeComponent::GetModuleHandle()
    {
        return sModuleHandle ? sModuleHandle : ::GetModuleHandle(0);
    }


    HWND NativeComponent::handle() const
    {
        return mHandle;
    }


    bool NativeComponent::initStyleControllers()
    {
        return Super::initStyleControllers();
    }


    bool NativeComponent::initAttributeControllers()
    {
        setAttributeController<DisabledController>(this);
        setAttributeController<LabelController>(this);
        return Super::initAttributeControllers();
    }


    bool NativeComponent::addEventListener(EventListener * inEventListener)
    {
        EventListeners::iterator it = mEventListeners.find(inEventListener);
        if (it == mEventListeners.end())
        {
            mEventListeners.insert(inEventListener);
            return true;
        }
        return false;
    }


    bool NativeComponent::removeEventListener(EventListener * inEventListener)
    {
        EventListeners::iterator it = mEventListeners.find(inEventListener);
        if (it != mEventListeners.end())
        {
            mEventListeners.erase(it);
            return true;
        }
        return false;
    }


    bool NativeComponent::getCustomBrush(HDC inHDC, HWND inHWND, HBRUSH & outHBRUSH)
    {
        if (mCSSBackgroundColor.isValid())
        {
            COLORREF colorRef = RGB(mCSSBackgroundColor.getValue().red(),
                                    mCSSBackgroundColor.getValue().green(),
                                    mCSSBackgroundColor.getValue().blue());
            outHBRUSH = ::CreateSolidBrush(colorRef);
            return true;
        }
        return false;
    }


    void NativeComponent::handleCommand(WPARAM wParam, LPARAM lParam)
    {
        unsigned short notificationCode = HIWORD(wParam);
        EventListeners::iterator it = mEventListeners.begin(), end = mEventListeners.end();
        for (; it != end; ++it)
        {
            (*it)->handleCommand(el(), notificationCode, wParam, lParam);
        }
    }


    void NativeComponent::handleMenuCommand(WORD inMenuId)
    {
        EventListeners::iterator it = mEventListeners.begin(), end = mEventListeners.end();
        for (; it != end; ++it)
        {
            (*it)->handleMenuCommand(el(), inMenuId);
        }
    }


    void NativeComponent::handleDialogCommand(WORD inNotificationCode, WPARAM wParam, LPARAM lParam)
    {
        EventListeners::iterator it = mEventListeners.begin(), end = mEventListeners.end();
        for (; it != end; ++it)
        {
            (*it)->handleDialogCommand(el(), inNotificationCode, wParam, lParam);
        }
    }


    LRESULT NativeComponent::handleMessage(UINT inMessage, WPARAM wParam, LPARAM lParam)
    {
        switch (inMessage)
        {
            case WM_COMMAND:
            {
                if (lParam == 0) // menu or accelerator
                {
                    int menuId = LOWORD(wParam);
                    if (Menu * menu = Menu::FindById(menuId))
                    {
                        // TODO: what should we do here
                        assert("What is this?");
                    }
                    else if (MenuItem * menuItem = MenuItem::FindById(menuId))
                    {
                        handleMenuCommand(menuId);
                        return 0;
                    }
                }
                else
                {
                    WORD paramHi = HIWORD(wParam);
                    WORD paramLo = LOWORD(wParam);

                    switch (paramLo)
                    {
                        case IDOK:
                        case IDCANCEL:
                        case IDABORT:
                        case IDRETRY:
                        case IDIGNORE:
                        case IDYES:
                        case IDNO:
                        case IDHELP:
                        case IDTRYAGAIN:
                        case IDCONTINUE:
                        {
                            NativeComponent * focus = FindByHandle(::GetFocus());
                            if (focus)
                            {
                                focus->handleDialogCommand(paramLo, wParam, lParam);
                                return 0;
                            }
                            break;
                        }
                        default:
                        {
                            // NOTE TO SELF: don't use "FindById(LOWORD(wParam))" here
                            //               because that won't work for toolbar buttons.
                            NativeComponent * sender = FindByHandle((HWND)lParam);
                            if (sender)
                            {
                                sender->handleCommand(wParam, lParam);
                                return 0;
                            }
                            break;
                        }
                    }
                }
                break;
            }
            // These messages get forwarded to the child elements that produced them.
            case WM_VSCROLL:
            case WM_HSCROLL:
            {
                HWND handle = (HWND)lParam;
                ComponentsByHandle::iterator it = sComponentsByHandle.find(handle);
                if (it != sComponentsByHandle.end())
                {
                    it->second->handleMessage(inMessage, wParam, lParam);
                    return 0;
                }
                break;
            }
            case WM_ERASEBKGND:
            {
                if (XULWin::NativeComponent * sender = NativeComponent::FindByHandle(handle()))
                {
                    if (sender->mCSSBackgroundColor.isValid())
                    {
                        HDC hDC = (HDC)wParam;
                        RGBColor color = sender->getCSSBackgroundColor();
                        HBRUSH backgroundBrush = ::CreateSolidBrush(RGB(color.red(), color.green(), color.blue()));
                        HBRUSH oldBrush = (HBRUSH)::SelectObject(hDC, (HGDIOBJ)&backgroundBrush);
                        RECT rect;
                        ::GetClientRect(handle(), &rect);
                        //::GetClipBox(hDC, &rect);                        
                        ::PatBlt(hDC, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, PATCOPY);
                        ::SelectObject(hDC, (HGDIOBJ)&oldBrush);
                        return TRUE;
                    }
                }
                break;
            }
            case WM_CTLCOLORMSGBOX:
            case WM_CTLCOLOREDIT:
            case WM_CTLCOLORLISTBOX:
            case WM_CTLCOLORBTN:
            case WM_CTLCOLORDLG:
            case WM_CTLCOLORSCROLLBAR:
            case WM_CTLCOLORSTATIC:
            {
                HDC hDC = (HDC)wParam;
                HWND hSender = (HWND)lParam;
                if (XULWin::NativeComponent * sender = NativeComponent::FindByHandle(hSender))
                {
                    if (sender->mCSSBackgroundColor.isValid())
                    {
                        // NOTE: these two calls should both be used if we also want to set the foreground color.
                        //SetTextColor(hdcStatic, RGB(255,255,255));
                        //SetBkColor(hdcStatic, RGB(0,0,0));

                        RGBColor color = sender->getCSSBackgroundColor();
                        return (INT_PTR)::CreateSolidBrush(RGB(color.red(), color.green(), color.blue()));
                    }
                }
                break;                    
            }
        }

        // Forward to event handlers
        EventListeners::iterator it = mEventListeners.begin(), end = mEventListeners.end();
        bool handled = false;
        for (; it != end; ++it)
        {
            int result = (*it)->handleMessage(el(), inMessage, wParam, lParam);
            if (result == 0)
            {
                handled = true;
            }
        }

        if (handled)
        {
            return 0;
        }

        if (mOrigProc)
        {
            return ::CallWindowProc(mOrigProc, mHandle, inMessage, wParam, lParam);
        }
        else
        {
            return ::DefWindowProc(mHandle, inMessage, wParam, lParam);
        }
    }


    LRESULT CALLBACK NativeComponent::MessageHandler(HWND hWnd, UINT inMessage, WPARAM wParam, LPARAM lParam)
    {
        ComponentsByHandle::iterator it = sComponentsByHandle.find(hWnd);
        if (it != sComponentsByHandle.end())
        {
            return it->second->handleMessage(inMessage, wParam, lParam);
        }
        return ::DefWindowProc(hWnd, inMessage, wParam, lParam);
    }

} // namespace XULWin
