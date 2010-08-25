#ifndef WINDOW_H_INCLUDED
#define WINDOW_H_INCLUDED


#include "XULWin/Component.h"
#include "XULWin/BoxLayouter.h"
#include "XULWin/Enums.h"
#include "XULWin/NativeComponent.h"
#include "XULWin/XMLWindow.h"
#include <boost/scoped_ptr.hpp>


namespace XULWin
{


    class Dialog;
    class Menu;

    class Window : public NativeComponent,
                   public virtual TitleController,
                   public BoxLayouter::ContentProvider
    {
    public:
        typedef NativeComponent Super;

        static void Register(HMODULE inModuleHandle);

        Window(const AttributesMapping & inAttr);

        virtual ~Window();

        virtual bool init();

        // TitleController methods
        virtual std::string getTitle() const;

        virtual void setTitle(const std::string & inTitle);

        virtual const Component * getChild(size_t idx) const;

        virtual Component * getChild(size_t idx);

        virtual void rebuildChildLayouts()
        {
            return Super::rebuildChildLayouts();
        }

        void show(WindowPos inPositioning);

        void showModal(WindowPos inPositioning);

        void close();

        virtual void move(int x, int y, int w, int h);

        virtual void rebuildLayout();

        virtual int calculateWidth(SizeConstraint inSizeConstraint) const;

        virtual int calculateHeight(SizeConstraint inSizeConstraint) const;

        virtual Rect clientRect() const;

        virtual Rect windowRect() const;

        virtual bool initAttributeControllers();

        virtual LRESULT handleMessage(UINT inMessage, WPARAM wParam, LPARAM lParam);

        virtual Orient BoxLayouter_getOrient() const;

        virtual Align BoxLayouter_getAlign() const;

        virtual size_t BoxLayouter_getChildCount() const;

        virtual const Component * BoxLayouter_getChild(size_t idx) const;

        virtual Component * BoxLayouter_getChild(size_t idx);

        virtual Rect BoxLayouter_clientRect() const;

        virtual void BoxLayouter_rebuildChildLayouts();

#ifndef SWIG
        static LRESULT CALLBACK MessageHandler(HWND hWnd, UINT inMessage, WPARAM wParam, LPARAM lParam);
#endif

    private:
        friend class Dialog;
        void setBlockingDialog(Dialog * inDlg);
        Dialog * mActiveDialog;
        boost::scoped_ptr<BoxLayouter> mBoxLayouter;
        bool mHasMessageLoop;
    };


} // namespace XULWin


#endif // WINDOW_H_INCLUDED
