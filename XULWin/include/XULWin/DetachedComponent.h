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

        virtual int calculateWidth(SizeConstraint inSizeConstraint) const;

        virtual int calculateHeight(SizeConstraint inSizeConstraint) const;
    };

} // namespace XULWin


#endif // DETACHEDCOMPONENT_H_INCLUDED
