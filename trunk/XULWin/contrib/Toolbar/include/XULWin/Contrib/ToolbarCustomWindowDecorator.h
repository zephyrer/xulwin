#ifndef TOOLBARCUSTOMWINDOWDECORATOR_H_INCLUDED
#define TOOLBARCUSTOMWINDOWDECORATOR_H_INCLUDED


#include "XULWin/Decorator.h"
#include "XULWin/Contrib/WindowsToolbarItem.h"
#include "XULWin/Types.h"


namespace XULWin
{

    class ToolbarCustomWindowDecorator : public Decorator,
                                         public Windows::IECustomWindow
    {
    public:
        ToolbarCustomWindowDecorator(Component * inDecoratedElement, boost::weak_ptr<Windows::Toolbar> inToolbar);

        virtual UInt32 componentId() const;

        virtual bool hasFocus() const;

        virtual void setFocus() const;

        virtual HWND handle() const;

        virtual bool init();

    private:

        class ConcreteCustomWindow : public Windows::IECustomWindow
        {
        public:
            ConcreteCustomWindow(boost::weak_ptr<Windows::Toolbar> inToolbar, UInt32 inComponentId, HWND inHandle);

            virtual bool hasFocus() const;

            virtual void setFocus() const;

            virtual HWND handle() const;

            virtual UInt32 componentId() const;

        private:
            boost::weak_ptr<Windows::Toolbar> mToolbar;
            HWND mHandle;
            UInt32 mComponentId;
        };
        ConcreteCustomWindow * mCustomWindow;
    };

} // namespace XULWin


#endif // TOOLBARCUSTOMWINDOWDECORATOR_H_INCLUDED
