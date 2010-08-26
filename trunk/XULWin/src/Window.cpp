#include "XULWin/Window.h"
#include "XULWin/Decorator.h"
#include "XULWin/Defaults.h"
#include "XULWin/Dialog.h"
#include "XULWin/ErrorReporter.h"
#include "XULWin/Menu.h"
#include "XULWin/WinUtils.h"
#include "XULWin/XULRunner.h"


namespace XULWin
{


    void Window::Register(HMODULE inModuleHandle)
    {
        WNDCLASSEX wndClass;
        wndClass.cbSize = sizeof(wndClass);
        wndClass.style = 0;
        wndClass.lpfnWndProc = &Window::MessageHandler;
        wndClass.cbClsExtra = 0;
        wndClass.cbWndExtra = 0;
        wndClass.hInstance = inModuleHandle;
        wndClass.hIcon = 0;
        wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
        wndClass.hbrBackground = (HBRUSH)(COLOR_BTNFACE+1);
        wndClass.lpszMenuName = NULL;
        wndClass.lpszClassName = TEXT("XULWin::XMLWindow");
        wndClass.hIconSm = 0;
        if (! RegisterClassEx(&wndClass))
        {
            ReportError("Could not register XUL::XMLWindow class.");
        }
    }


    Window::Window(const AttributesMapping & inAttr) :
        NativeComponent(0, inAttr),
        mActiveDialog(0),
        mHasMessageLoop(false)
    {
        mBoxLayouter.reset(new BoxLayouter(this));
        mHandle = ::CreateWindowEx
                  (
                      0,
                      TEXT("XULWin::XMLWindow"),
                      TEXT(""),
                      WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_OVERLAPPEDWINDOW,
                      CW_USEDEFAULT, CW_USEDEFAULT, Defaults::windowWidth(), Defaults::windowHeight(),
                      0,
                      (HMENU)0, // must be zero if not menu and not child
                      mModuleHandle,
                      0
                  );

        std::string error = WinAPI::getLastError(::GetLastError());


        // set default font
        ::SendMessage(mHandle, WM_SETFONT, (WPARAM)::GetStockObject(DEFAULT_GUI_FONT), MAKELPARAM(FALSE, 0));
        registerHandle();
    }


    Window::~Window()
    {
    }


    bool Window::init()
    {
        // Set the small icon in the upper left corner.
        if (HICON hIcon = XULRunner::GetDefaultIcon(el()->getAttribute("id")))
        {
            ::SendMessage(handle(), WM_SETICON, ICON_BIG, (LPARAM)hIcon);
        }
        return Super::init();
    }


    bool Window::initAttributeControllers()
    {
        setAttributeController<TitleController>(this);
        return Super::initAttributeControllers();
    }


    Rect Window::clientRect() const
    {
        RECT rc;
        ::GetClientRect(handle(), &rc);
        return Rect(rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top);
    }


    Rect Window::windowRect() const
    {
        RECT rw;
        ::GetWindowRect(handle(), &rw);
        return Rect(rw.left, rw.top, rw.right - rw.left, rw.bottom - rw.top);
    }


    Orient Window::BoxLayouter_getOrient() const
    {
        return getOrient();
    }


    Align Window::BoxLayouter_getAlign() const
    {
        return getAlign();
    }


    size_t Window::BoxLayouter_getChildCount() const
    {
        size_t result = getChildCount();
        if (const MenuBar * menuBar = findChildOfType<MenuBar>())
        {
            result -= 1;
        }
        return result;
    }


    const Component * Window::BoxLayouter_getChild(size_t idx) const
    {
        if (const MenuBar * menuBar = findChildOfType<MenuBar>())
        {
            return getChild(idx + 1);
        }
        return getChild(idx);
    }


    Component * Window::BoxLayouter_getChild(size_t idx)
    {
        if (const MenuBar * menuBar = findChildOfType<MenuBar>())
        {
            return getChild(idx + 1);
        }
        return getChild(idx);
    }


    Rect Window::BoxLayouter_clientRect() const
    {
        return clientRect();
    }


    void Window::BoxLayouter_rebuildChildLayouts()
    {
        rebuildChildLayouts();
    }


    int Window::calculateWidth(SizeConstraint inSizeConstraint) const
    {
        return getOrient() == Horizontal ? calculateSumChildWidths(inSizeConstraint)
               : calculateMaxChildWidth(inSizeConstraint);
    }


    int Window::calculateHeight(SizeConstraint inSizeConstraint) const
    {
        int result = 0;
        if (getOrient() == Vertical)
        {
            result = calculateSumChildHeights(inSizeConstraint);
        }
        else
        {
            result = calculateMaxChildHeight(inSizeConstraint);
        }
        return result;
    }


    void Window::move(int x, int y, int w, int h)
    {
        ::MoveWindow(handle(), x, y, w, h, FALSE);
    }


    void Window::rebuildLayout()
    {
        mBoxLayouter->rebuildLayout();
    }


    std::string Window::getTitle() const
    {
        return WinAPI::getWindowText(handle());
    }


    void Window::setTitle(const std::string & inTitle)
    {
        WinAPI::setWindowText(handle(), inTitle);
    }


    const Component * Window::getChild(size_t idx) const
    {
        if (el())
        {
            return el()->children()[idx]->component();
        }
        return 0;
    }


    Component * Window::getChild(size_t idx)
    {
        if (el())
        {
            return el()->children()[idx]->component();
        }
        return 0;
    }


    void Window::showModal(WindowPos inPositioning)
    {
        show(inPositioning);

        mHasMessageLoop = true;
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
    }


    void Window::show(WindowPos inPositioning)
    {
        rebuildLayout();

        if (inPositioning == WindowPos_CenterInScreen)
        {
            int w = getWidth();
            int h = getHeight();
            int x = (GetSystemMetrics(SM_CXSCREEN) - w)/2;
            int y = (GetSystemMetrics(SM_CYSCREEN) - h)/2;
            move(x, y, w, h);
        }
        else
        {
            int w = getWidth();
            int h = getHeight();
            int x = getCSSX();
            int y = getCSSY();
            move(x, y, w, h);
        }

        ::ShowWindow(handle(), SW_SHOW);
        ::UpdateWindow(handle());
    }


    void Window::close()
    {
        setHidden(true);
        if (mHasMessageLoop)
        {
            ::PostQuitMessage(0);
        }
    }


    LRESULT Window::handleMessage(UINT inMessage, WPARAM wParam, LPARAM lParam)
    {
        switch (inMessage)
        {
            case WM_SETFOCUS:
            {
                if (mActiveDialog)
                {
                    ::SetFocus(mActiveDialog->handle());
                    return 0;
                }
                break;
            }
            case WM_SIZE:
            {
                rebuildLayout();
                invalidateRect();
                return 0;
            }
            case WM_CLOSE:
            {
                close();
                return 0;
            }
            case WM_ERASEBKGND:
            {
                if (mCSSBackgroundColor.isValid())
                {
                    // The WM_PAINT message will take care of painting the background.
                    return 1;
                }
                break;
            }
            case WM_PAINT:
            {
                if (mCSSBackgroundColor.isValid())
                {
                    HDC hDC = ::GetDC(handle());
                    PAINTSTRUCT ps;
                    ps.hdc = hDC;
                    ::BeginPaint(handle(), &ps);
                    RECT rc;
                    ::GetClientRect(handle(), &rc);
                    RGBColor color = getCSSBackgroundColor();
                    HBRUSH bgBrush = ::CreateSolidBrush(RGB(color.red(), color.green(), color.blue()));
                    ::FillRect(hDC, &rc, bgBrush);
                    ::EndPaint(handle(), &ps);
                    ::ReleaseDC(handle(), hDC);

                    // An application returns zero if it processes this message.
                    return 0;
                }
                break;
            }
            case WM_GETMINMAXINFO:
            {
                SIZE sizeDiff = WinAPI::getSizeDifferenceBetweenWindowRectAndClientRect(handle());
                MINMAXINFO * minMaxInfo = (MINMAXINFO *)lParam;
                minMaxInfo->ptMinTrackSize.x = getWidth(Minimum) + sizeDiff.cx;
                minMaxInfo->ptMinTrackSize.y = getHeight(Minimum) + sizeDiff.cy;
                return 0;
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


        bool handled = false;

        // Forward to event handlers
        EventListeners::iterator it = mEventListeners.begin(), end = mEventListeners.end();
        for (; it != end; ++it)
        {
            LRESULT result = (*it)->handleMessage(el(), inMessage, wParam, lParam);
            if (0 == result)
            {
                handled = true;
            }
        }

        if (handled)
        {
            return 0;
        }
        else
        {
            return Super::handleMessage(inMessage, wParam, lParam);
        }
    }


    LRESULT CALLBACK Window::MessageHandler(HWND hWnd, UINT inMessage, WPARAM wParam, LPARAM lParam)
    {
        NativeComponent * sender = FindByHandle(hWnd);
        if (sender)
        {
            int result = sender->handleMessage(inMessage, wParam, lParam);
            if (result == 0)
            {
                return 0;
            }
            else if (result == 1)
            {
                return ::DefWindowProc(hWnd, inMessage, wParam, lParam);
            }
            else
            {
                return result;
            }
        }
        return ::DefWindowProc(hWnd, inMessage, wParam, lParam);
    }


    void Window::setBlockingDialog(Dialog * inDlg)
    {
        mActiveDialog = inDlg;
    }


} // namespace XULWin
