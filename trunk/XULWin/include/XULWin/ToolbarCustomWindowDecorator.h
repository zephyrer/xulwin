#ifndef TOOLBARCUSTOMWINDOWDECORATOR_H_INCLUDED
#define TOOLBARCUSTOMWINDOWDECORATOR_H_INCLUDED


#include "XULWin/Decorator.h"
#include "XULWin/ToolbarItem.h"


namespace XULWin
{

    class ToolbarCustomWindowDecorator : public Decorator,
                                         public Windows::IECustomWindow
    {
    public:
        ToolbarCustomWindowDecorator(Component * inDecoratedElement, boost::weak_ptr<Windows::Toolbar> inToolbar);

        virtual int commandId() const;

		virtual bool hasFocus() const;

		virtual void setFocus() const;

		virtual HWND handle() const;

        virtual bool initImpl();

    private:
    
        class ConcreteCustomWindow : public Windows::IECustomWindow
        {
        public:
            ConcreteCustomWindow(boost::weak_ptr<Windows::Toolbar> inToolbar, int inCommandId, HWND inHandle);

            virtual bool hasFocus() const;

		    virtual void setFocus() const;

		    virtual HWND handle() const;

            virtual int commandId() const;

        private:
            boost::weak_ptr<Windows::Toolbar> mToolbar;
            HWND mHandle;
            int mCommandId;
        };
        ConcreteCustomWindow * mCustomWindow;
    };

} // namespace XULWin


#endif // TOOLBARCUSTOMWINDOWDECORATOR_H_INCLUDED
