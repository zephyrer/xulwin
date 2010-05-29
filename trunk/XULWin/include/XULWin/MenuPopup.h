#ifndef MENUPOPUPCOMPONENT_H_INCLUDED
#define MENUPOPUPCOMPONENT_H_INCLUDED


#include "XULWin/PhonyComponent.h"
#include "XULWin/PopupMenu.h"


namespace XULWin
{


    class MenuPopup : public PhonyComponent
    {
    public:
        typedef PhonyComponent Super;

        MenuPopup(Component * inParent, const AttributesMapping & inAttr);

        void show(RECT inExcludeRect);

        virtual int calculateWidth(SizeConstraint inSizeConstraint) const;

        virtual int calculateHeight(SizeConstraint inSizeConstraint) const;

    protected:
        virtual void onChildAdded(Component * inChild);

        virtual void onChildRemoved(Component * inChild);

    private:
        Windows::PopupMenu * getMenu();
    };

} // namespace XULWin


#endif // MENUPOPUPCOMPONENT_H_INCLUDED
