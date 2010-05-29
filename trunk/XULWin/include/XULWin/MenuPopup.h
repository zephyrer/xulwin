#ifndef MENUPOPUPCOMPONENT_H_INCLUDED
#define MENUPOPUPCOMPONENT_H_INCLUDED


#include "XULWin/Component.h"
#include "XULWin/Components.h"


namespace XULWin
{


    class MenuPopup : public DummyComponent
    {
    public:
        typedef DummyComponent Super;

        MenuPopup(Component * inParent, const AttributesMapping & inAttributesMapping);

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
