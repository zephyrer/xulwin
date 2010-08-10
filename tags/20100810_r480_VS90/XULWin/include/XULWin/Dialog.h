#ifndef DIALOG_H_INCLUDED
#define DIALOG_H_INCLUDED


#include "XULWin/BoxLayouter.h"
#include "XULWin/NativeComponent.h"
#include <boost/scoped_ptr.hpp>


namespace XULWin
{

    class Window;

    /**
     * Dialog
     *
     * Represents a XUL dialog window.
     * In reality it is a Window with some customizations for Dialog-like behavior.
     */
    class Dialog : public NativeComponent,
                   public BoxLayouter::ContentProvider,
                   public virtual TitleController
    {
    public:
        typedef NativeComponent Super;

        static void Register(HMODULE inModuleHandle);

        Dialog(Component * inParent, const AttributesMapping & inAttr);

        virtual ~Dialog();

        // BoxLayouter
        virtual Orient getOrient() const;

        // BoxLayouter
        virtual Align getAlign() const;

        // TitleController methods
        virtual std::string getTitle() const;

        virtual void setTitle(const std::string & inTitle);

        virtual const Component * getChild(size_t idx) const;

        virtual Component * getChild(size_t idx);

        virtual void rebuildChildLayouts()
        {
            return Super::rebuildChildLayouts();
        }

        DialogResult showModal(Window * inInvoker);

        LRESULT endModal(DialogResult inDialogResult);

        virtual void move(int x, int y, int w, int h);

        virtual void rebuildLayout();

        virtual int calculateWidth(SizeConstraint inSizeConstraint) const;

        virtual int calculateHeight(SizeConstraint inSizeConstraint) const;

        virtual Rect clientRect() const;

        virtual Rect windowRect() const;

        virtual bool initAttributeControllers();

        virtual bool initStyleControllers();

        virtual Orient BoxLayouter_getOrient() const
        {
            return getOrient();
        }

        virtual Align BoxLayouter_getAlign() const
        {
            return getAlign();
        }

        virtual size_t BoxLayouter_getChildCount() const
        {
            return getChildCount();
        }

        virtual const Component * BoxLayouter_getChild(size_t idx) const
        {
            return getChild(idx);
        }

        virtual Component * BoxLayouter_getChild(size_t idx)
        {
            return getChild(idx);
        }

        virtual Rect BoxLayouter_clientRect() const
        {
            return clientRect();
        }

        virtual void BoxLayouter_rebuildChildLayouts()
        {
            rebuildChildLayouts();
        }

        virtual LRESULT handleMessage(UINT inMessage, WPARAM wParam, LPARAM lParam);

        static LRESULT CALLBACK MessageHandler(HWND hWnd, UINT inMessage, WPARAM wParam, LPARAM lParam);
    private:
        // Invoker is the stored parameter for showModal.
        Window * mInvoker;
        boost::scoped_ptr<BoxLayouter> mBoxLayouter;
        DialogResult mDialogResult;
    };

} // namespace XULWin


#endif // DIALOG_H_INCLUDED
