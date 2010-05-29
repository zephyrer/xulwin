#ifndef TOOLBAR_H_INCLUDED
#define TOOLBAR_H_INCLUDED


#include "XULWin/NativeControl.h"
#include "XULWin/GdiplusLoader.h"
#include "XULWin/WindowsToolbar.h"


namespace XULWin
{

    class Toolbar : public NativeControl,
                    public Windows::Toolbar::EventHandler,
                    public GdiplusLoader
    {
    public:
        typedef NativeControl Super;

        Toolbar(Component * inParent, const AttributesMapping & inAttributesMapping);

        virtual ~Toolbar();

        virtual bool init();

        virtual int calculateWidth(SizeConstraint inSizeConstraint) const;

        virtual int calculateHeight(SizeConstraint inSizeConstraint) const;

        virtual void rebuildLayout();

        // ToolbarElement::EventHandler methods
        virtual void onRequestFocus() {}

        boost::shared_ptr<Windows::Toolbar> nativeToolbar() const
        {
            return mToolbar;
        }

    private:
        boost::shared_ptr<Windows::Toolbar> mToolbar;
    };

} // namespace XULWin


#endif // TOOLBAR_H_INCLUDED
