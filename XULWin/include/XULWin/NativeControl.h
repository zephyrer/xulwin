#ifndef NATIVECONTROL_H_INCLUDED
#define NATIVECONTROL_H_INCLUDED


#include "XULWin/NativeComponent.h"


namespace XULWin
{

    /**
     * NativeControl
     *
     * A native control is a non-container component that
     * maps to a native HWND. It is the base class for the
     * native WinAPI controls.
     */
    class NativeControl : public NativeComponent
    {
    public:
        typedef NativeComponent Super;

        NativeControl(Component * inParent, const AttributesMapping & inAttr, LPCTSTR inClassName, DWORD inExStyle, DWORD inStyle);

        // use this constructor if you want to provide your own handle later using NativeControl::setHandle
        NativeControl(Component * inParent, const AttributesMapping & inAttr);

        virtual ~NativeControl();

        bool initStyleControllers();

        virtual void rebuildLayout();

        virtual Rect clientRect() const;

        virtual void move(int x, int y, int w, int h);

        // Returns the first native parent component.
        static NativeComponent * FindNativeParent(Component * inComponent);
        static const NativeComponent * FindNativeParent(const Component * inComponent);
    };

} // namespace XULWin


#endif // NATIVECONTROL_H_INCLUDED
