#include "XULWin/Dialog.h"
#include "XULWin/Decorator.h"
#include "XULWin/Element.h"
#include "XULWin/Defaults.h"
#include "XULWin/ErrorReporter.h"
#include "XULWin/EventListener.h"
#include "XULWin/Window.h"
#include "XULWin/WinUtils.h"


namespace XULWin
{

    void Dialog::Register(HMODULE inModuleHandle)
    {
        WNDCLASSEX wndClass;
        wndClass.cbSize = sizeof(wndClass);
        wndClass.style = 0;
        wndClass.lpfnWndProc = &Dialog::MessageHandler;
        wndClass.cbClsExtra = 0;
        wndClass.cbWndExtra = 0;
        wndClass.hInstance = inModuleHandle;
        wndClass.hIcon = 0;
        wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
        wndClass.hbrBackground = (HBRUSH)(COLOR_BTNFACE+1);
        wndClass.lpszMenuName = NULL;
        wndClass.lpszClassName = TEXT("XULWin::DialogElement");
        wndClass.hIconSm = 0;
        if (! RegisterClassEx(&wndClass))
        {
            ReportError("Could not register XUL::DialogElement class.");
        }
    }


    namespace
    {
        BOOL CALLBACK DisableAllExcept(HWND inWindow, LPARAM lParam)
        {
            HWND exceptThisOne = (HWND)lParam;
            if (inWindow != exceptThisOne)
            {
                ::EnableWindow(inWindow, false);
            }
            return true;
        }

        BOOL CALLBACK EnableAllExcept(HWND inWindow, LPARAM lParam)
        {
            HWND exceptThisOne = (HWND)lParam;
            if (inWindow != exceptThisOne)
            {
                ::EnableWindow(inWindow, true);
            }
            return true;
        }
    }


    Dialog::Dialog(Component * inParent, const AttributesMapping & inAttributesMapping) :
        NativeComponent(inParent, inAttributesMapping),
        mBoxLayouter(this),
        mInvoker(0),
        mDialogResult(DialogResult_Cancel)
    {
        if (NativeComponent * comp = inParent->downcast<NativeComponent>())
        {
            mHandle = ::CreateWindowEx
                      (
                          0,
                          TEXT("XULWin::DialogElement"),
                          TEXT(""),
                          WS_POPUPWINDOW | WS_CAPTION | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
                          CW_USEDEFAULT, CW_USEDEFAULT, Defaults::windowWidth(), Defaults::windowHeight(),
                          comp->handle(),
                          (HMENU)0, // must be zero if not menu and not child
                          mModuleHandle,
                          0
                      );
        }

        std::string error = Windows::getLastError(::GetLastError());


        // set default font
        ::SendMessage(mHandle, WM_SETFONT, (WPARAM)::GetStockObject(DEFAULT_GUI_FONT), MAKELPARAM(FALSE, 0));
        registerHandle();
    }


    Dialog::~Dialog()
    {
    }


    bool Dialog::initStyleControllers()
    {
        return Super::initStyleControllers();
    }


    bool Dialog::initAttributeControllers()
    {
        setAttributeController<TitleController>(this);
        return Super::initAttributeControllers();
    }


    Rect Dialog::clientRect() const
    {
        RECT rc;
        ::GetClientRect(handle(), &rc);
        return Rect(rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top);
    }


    Rect Dialog::windowRect() const
    {
        RECT rw;
        ::GetWindowRect(handle(), &rw);
        return Rect(rw.left, rw.top, rw.right - rw.left, rw.bottom - rw.top);
    }


    int Dialog::calculateWidth(SizeConstraint inSizeConstraint) const
    {
        return getOrient() == Horizontal ? calculateSumChildWidths(inSizeConstraint)
               : calculateMaxChildWidth(inSizeConstraint);
    }


    int Dialog::calculateHeight(SizeConstraint inSizeConstraint) const
    {
        return getOrient() == Vertical ? calculateSumChildHeights(inSizeConstraint)
               : calculateMaxChildHeight(inSizeConstraint);
    }


    void Dialog::move(int x, int y, int w, int h)
    {
        ::MoveWindow(handle(), x, y, w, h, FALSE);
    }


    void Dialog::rebuildLayout()
    {
        mBoxLayouter.rebuildLayout();
    }


    Orient Dialog::getOrient() const
    {
        return Super::getOrient();
    }


    Align Dialog::getAlign() const
    {
        return Super::getAlign();
    }


    std::string Dialog::getTitle() const
    {
        return Windows::getWindowText(handle());
    }


    void Dialog::setTitle(const std::string & inTitle)
    {
        Windows::setWindowText(handle(), inTitle);
    }


    const Component * Dialog::getChild(size_t idx) const
    {
        if (el())
        {
            const Children & children = el()->children();
            if (idx < children.size())
            {
                return children[idx]->component();
            }
        }
        return 0;
    }


    Component * Dialog::getChild(size_t idx)
    {
        return const_cast<Component *>(static_cast<const Dialog *>(this)->getChild(idx));
    }


    DialogResult Dialog::showModal(Window * inInvoker)
    {
        mInvoker = inInvoker;
        if (mInvoker)
        {
            mInvoker->setBlockingDialog(this);
        }

        // Disable all windows except the dialog
        DWORD threadID = GetWindowThreadProcessId(handle(), NULL);
        ::EnumThreadWindows(threadID, &DisableAllExcept, (LPARAM)handle());


        rebuildLayout();
        SIZE sz = Windows::getSizeDifferenceBetweenWindowRectAndClientRect(handle());
        int w = getWidth() + sz.cx;
        int h = getHeight() + sz.cy;
        int x = (GetSystemMetrics(SM_CXSCREEN) - w)/2;
        int y = (GetSystemMetrics(SM_CYSCREEN) - h)/2;
        move(x, y, w, h);
        ::ShowWindow(handle(), SW_SHOW);
        ::UpdateWindow(handle());

        MSG message;
        while (GetMessage(&message, NULL, 0, 0))
        {
            HWND hActive = GetActiveWindow();
            if (! IsDialogMessage(hActive, &message))
            {
                TranslateMessage(&message);
                DispatchMessage(&message);
            }
        }
        return mDialogResult;
    }


    LRESULT Dialog::endModal(DialogResult inDialogResult)
    {
        mDialogResult = inDialogResult;
        // Re-enable all windows
        DWORD threadID = GetWindowThreadProcessId(handle(), NULL);
        ::EnumThreadWindows(threadID, &EnableAllExcept, (LPARAM)handle());
        if (mInvoker)
        {
            mInvoker->setBlockingDialog(0);
            mInvoker = 0;
        }

        setHidden(true);
        ::PostQuitMessage(0);
        return 0;
    }


    LRESULT Dialog::handleMessage(UINT inMessage, WPARAM wParam, LPARAM lParam)
    {
        switch (inMessage)
        {
            case WM_SIZE:
            {
                rebuildLayout();
                invalidateRect();
                return 0;
            }
            case WM_CLOSE:
            {
                endModal(DialogResult_Cancel);
                return 0;
            }
            case WM_GETMINMAXINFO:
            {
                SIZE sizeDiff = Windows::getSizeDifferenceBetweenWindowRectAndClientRect(handle());
                MINMAXINFO * minMaxInfo = (MINMAXINFO *)lParam;
                minMaxInfo->ptMinTrackSize.x = getWidth(Minimum) + sizeDiff.cx;
                minMaxInfo->ptMinTrackSize.y = getHeight(Minimum) + sizeDiff.cy;
                return 0;
            }
            case WM_COMMAND:
            {
                WORD paramHi = HIWORD(wParam);
                WORD paramLo = LOWORD(wParam);

                switch (paramLo)
                {
                    case IDYES:
                    case IDOK:
                    {
                        endModal(DialogResult_Ok);
                        return 0;
                    }
                    case IDCANCEL:
                    case IDNO:
                    {
                        endModal(DialogResult_Cancel);
                        return 0;
                    }
                    default:
                    {
                        NativeComponent * sender = FindById(LOWORD(wParam));
                        if (sender)
                        {
                            sender->handleCommand(wParam, lParam);
                            return 0;
                        }
                        break;
                    }
                }
                break;
            }
            // These messages get forwarded to the child elements that produced them.
            case WM_VSCROLL:
            case WM_HSCROLL:
            {
                NativeComponent * sender = FindByHandle((HWND)lParam);
                if (sender)
                {
                    sender->handleMessage(inMessage, wParam, lParam);
                    return 0;
                }
                break;
            }
        }

        // Forward to event handlers
        EventListeners::iterator it = mEventListeners.begin(), end = mEventListeners.end();
        for (; it != end; ++it)
        {
            (*it)->handleMessage(el(), inMessage, wParam, lParam);
        }
        return ::DefWindowProc(handle(), inMessage, wParam, lParam);
    }


    LRESULT CALLBACK Dialog::MessageHandler(HWND hWnd, UINT inMessage, WPARAM wParam, LPARAM lParam)
    {
        NativeComponent * sender = FindByHandle(hWnd);
        if (sender)
        {
            int result = sender->handleMessage(inMessage, wParam, lParam);
            if (result == 0)
            {
                return 0;
            }
        }
        return ::DefWindowProc(hWnd, inMessage, wParam, lParam);
    }

} // namespace XULWin
