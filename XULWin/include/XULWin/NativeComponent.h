#ifndef NATIVECOMPONENT_H_INCLUDED
#define NATIVECOMPONENT_H_INCLUDED


#include "XULWin/ConcreteComponent.h"
#include <set>


namespace XULWin
{

    class EventListener;

    /**
     * NativeComponent
     *
     * Native components are UI components that map to a native HWND.
     */
    class NativeComponent : public ConcreteComponent,
                            public virtual DisabledController,
                            public virtual LabelController
    {
    public:
        typedef ConcreteComponent Super;

        NativeComponent(Component * inParent, const AttributesMapping & inAttributes);

        virtual ~NativeComponent();

        virtual void invalidateRect() const;

        virtual void setHandle(HWND inHandle, bool inPassOwnership);

        bool addEventListener(EventListener * inEventListener);

        bool removeEventListener(EventListener * inEventListener);

        // DisabledController methods
        virtual bool isDisabled() const;

        virtual void setDisabled(bool inDisabled);

        // LabelController methods
        virtual std::string getLabel() const;

        virtual void setLabel(const std::string & inLabel);

        // HiddenController methods
        virtual void setHidden(bool inHidden);

        // Override component coloring.
        virtual bool getCustomBrush(HDC inHDC, HWND inHWND, HBRUSH & outHBRUSH);

        static void SetModuleHandle(HMODULE inModule);

        static HMODULE GetModuleHandle();

        virtual HWND handle() const;

        virtual bool initAttributeControllers();

        virtual bool initStyleControllers();

        virtual void handleCommand(WPARAM wParam, LPARAM lParam);

        virtual void handleMenuCommand(WORD inMenuId);

        virtual void handleDialogCommand(WORD inNotificationCode, WPARAM wParam, LPARAM lParam);

        virtual LRESULT handleMessage(UINT inMessage, WPARAM wParam, LPARAM lParam);

        static LRESULT CALLBACK MessageHandler(HWND hWnd, UINT inMessage, WPARAM wParam, LPARAM lParam);

    protected:
        static NativeComponent * FindByHandle(HWND inHandle);

        static NativeComponent * FindById(int inId);

        void registerHandle();

        void subclass();

        void unsubclass();

        void unregisterHandle();

        HWND mHandle;
        HMODULE mModuleHandle;


        typedef std::set<EventListener *> EventListeners;
        EventListeners mEventListeners;

    private:
        typedef std::map<int, NativeComponent *> ComponentsById;
        static ComponentsById sComponentsById;

        typedef std::map<HWND, NativeComponent *> ComponentsByHandle;
        static ComponentsByHandle sComponentsByHandle;

        WNDPROC mOrigProc;

        static HMODULE sModuleHandle;
        bool mOwnsHandle;
    };

} // namespace XULWin


#endif // NATIVECOMPONENT_H_INCLUDED
