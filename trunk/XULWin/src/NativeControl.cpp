#include "XULWin/NativeControl.h"
#include "XULWin/Decorator.h"
#include "XULWin/WinUtils.h"
#include "XULWin/VirtualComponent.h"


namespace XULWin
{

    NativeControl::NativeControl(Component * inParent, const AttributesMapping & inAttr, LPCTSTR inClassName, DWORD inExStyle, DWORD inStyle) :
        NativeComponent(inParent, inAttr)
    {
        if (!mParent)
        {
            throw std::runtime_error("NativeControl constructor failed because parent is NULL.");
        }

        Rect clientRect = inParent->clientRect();
        NativeComponent * nativeParent = GetThisOrParent(inParent);
        if (!nativeParent)
        {
            throw std::runtime_error("NativeControl constructor failed because no native parent was found.");
            return;
        }

        mHandle = ::CreateWindowEx(inExStyle,
                                   inClassName,
                                   TEXT(""),
                                   inStyle | WS_CHILD | WS_CLIPCHILDREN | WS_VISIBLE,
                                   0, 0, 0, 0,
                                   nativeParent->handle(),
                                   (HMENU)mComponentId.value(),
                                   mModuleHandle,
                                   0);

        if (!mHandle)
        {
            throw std::runtime_error(Windows::getLastError(::GetLastError()));
            return;
        }

        // set default font
        ::SendMessage(mHandle, WM_SETFONT, (WPARAM)::GetStockObject(DEFAULT_GUI_FONT), MAKELPARAM(FALSE, 0));

        registerHandle();
        subclass();
    }


    NativeControl::NativeControl(Component * inParent, const AttributesMapping & inAttr) :
        NativeComponent(inParent, inAttr)
    {
        // Don't call registerHandle() or subclass() here.
        // They have to be called in your subclass and after setHandle() has been called.
    }


    NativeControl::~NativeControl()
    {
    }


    bool NativeControl::initStyleControllers()
    {
        return Super::initStyleControllers();
    }


    void NativeControl::rebuildLayout()
    {
        rebuildChildLayouts();
    }


    void NativeControl::move(int x, int y, int w, int h)
    {
        if (NativeComponent * nativeParent = dynamic_cast<NativeComponent *>(parent()))
        {
            // This situation occurs if the scroll decorator created a STATIC window for
            // the scrollable rectangular area. This new context requires that we
            // re-adjust the x and y coords.
            Rect scrollRect = nativeParent->clientRect();
            ::MoveWindow(handle(), x - scrollRect.x(), y - scrollRect.y(), w, h, FALSE);
        }
        else
        {
            // If the parent is a virtual element, then we can position this control normally.
            ::MoveWindow(handle(), x, y, w, h, FALSE);
        }
    }


    Rect NativeControl::clientRect() const
    {
        HWND hwndParent = ::GetParent(handle());
        if (!hwndParent)
        {
            RECT rc;
            ::GetClientRect(handle(), &rc);
        }

        RECT rc_parent;
        ::GetClientRect(hwndParent, &rc_parent);
        ::MapWindowPoints(hwndParent, HWND_DESKTOP, (LPPOINT)&rc_parent, 2);

        RECT rc_self;
        ::GetClientRect(handle(), &rc_self);
        ::MapWindowPoints(handle(), HWND_DESKTOP, (LPPOINT)&rc_self, 2);


        int x = rc_self.left - rc_parent.left;
        int y = rc_self.top - rc_parent.top;
        return Rect(x, y, rc_self.right - rc_self.left, rc_self.bottom - rc_self.top);
    }


    const NativeComponent * NativeControl::GetThisOrParent(const Component * inElement)
    {
        if (const NativeComponent * obj = dynamic_cast<const NativeComponent *>(inElement))
        {
            return obj;
        }
        else if (const Decorator * obj = dynamic_cast<const Decorator *>(inElement))
        {
            return GetThisOrParent(obj->decoratedElement().get());
        }
        else if (const VirtualComponent * obj = dynamic_cast<const VirtualComponent *>(inElement))
        {
            return GetThisOrParent(obj->parent());
        }
        return 0;
    }


    NativeComponent * NativeControl::GetThisOrParent(Component * inElement)
    {
        return const_cast<NativeComponent *>(GetThisOrParent(const_cast<const Component *>(inElement)));
    }

} // namespace XULWin
