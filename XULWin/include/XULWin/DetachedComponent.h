#ifndef DETACHEDCOMPONENT_H_INCLUDED
#define DETACHEDCOMPONENT_H_INCLUDED


#include "XULWin/ConcreteComponent.h"


namespace XULWin
{

    /**
     * Detached components are isolated controls that are only
     * visible for short periods and independent from layout managers.
     * Examples are menus, tooltips, startup screens, etc...
     */
    class DetachedComponent : public ConcreteComponent
    {
    public:
        typedef ConcreteComponent Super;

        DetachedComponent(Component * inParent, const AttributesMapping & inAttr);

        virtual ~DetachedComponent();

        virtual void move(int x, int y, int w, int h);

        virtual Rect clientRect() const;

        virtual void rebuildLayout();

        virtual LRESULT handleMessage(UINT inMessage, WPARAM wParam, LPARAM lParam);

        virtual int calculateWidth(SizeConstraint inSizeConstraint) const;

        virtual int calculateHeight(SizeConstraint inSizeConstraint) const;

    private:
        Rect mRect;
    };

} // namespace XULWin


#endif // DETACHEDCOMPONENT_H_INCLUDED
