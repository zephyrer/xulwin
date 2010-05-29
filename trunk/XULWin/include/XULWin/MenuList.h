#ifndef MENULIST_H_INCLUDED
#define MENULIST_H_INCLUDED


#include "XULWin/NativeControl.h"
#include "XULWin/AttributesMapping.h"


namespace XULWin
{

  class MenuList : public NativeControl
    {
    public:
        typedef NativeControl Super;

        MenuList(Component * inParent, const AttributesMapping & inAttr);

        virtual bool init();

        virtual int calculateWidth(SizeConstraint inSizeConstraint) const;

        virtual int calculateHeight(SizeConstraint inSizeConstraint) const;

        virtual void move(int x, int y, int w, int h);

        virtual void onContentChanged();

    private:
        void fillComboBox();
    };

} // namespace XULWin


#endif // MENULIST_H_INCLUDED
