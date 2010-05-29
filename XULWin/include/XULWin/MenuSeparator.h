#ifndef MENUSEPARATOR_H_INCLUDED
#define MENUSEPARATOR_H_INCLUDED


#include "XULWin/Component.h"
#include "XULWin/VirtualComponent.h"


namespace XULWin
{

    class MenuSeparator : public VirtualComponent
    {
    public:
        typedef VirtualComponent Super;

        MenuSeparator(Component * inParent, const AttributesMapping & inAttr);

        virtual int calculateWidth(SizeConstraint inSizeConstraint) const { return 0; }

        virtual int calculateHeight(SizeConstraint inSizeConstraint) const { return 0; }
    };

} // namespace XULWin


#endif // MENUSEPARATOR_H_INCLUDED
