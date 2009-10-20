#ifndef MENUPOPUPCOMPONENT_H_INCLUDED
#define MENUPOPUPCOMPONENT_H_INCLUDED


#include "XULWin/Component.h"


namespace XULWin
{

    class MenuPopup : public PassiveComponent
    {
    public:
        typedef PassiveComponent Super;

        MenuPopup(Component * inParent, const AttributesMapping & inAttributesMapping);

        void show(RECT inExcludeRect);

    protected:
        virtual void onChildAdded(Component * inChild);

        virtual void onChildRemoved(Component * inChild);

    private:
        Windows::PopupMenu * getMenu();
    };

} // namespace XULWin


#endif // MENUPOPUPCOMPONENT_H_INCLUDED
