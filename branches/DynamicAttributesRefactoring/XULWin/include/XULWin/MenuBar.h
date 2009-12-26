#ifndef MENUBARCOMPONENT_H_INCLUDED
#define MENUBARCOMPONENT_H_INCLUDED


#include "XULWin/Component.h"


namespace XULWin
{

    class MenuBar : public PassiveComponent
    {
    public:
        typedef PassiveComponent Super;

        MenuBar(Component * inParent, const AttributesMapping & inAttributesMapping);

        virtual bool init();

        virtual int calculateWidth(SizeConstraint inSizeConstraint) const;

        virtual int calculateHeight(SizeConstraint inSizeConstraint) const;
    };

} // namespace XULWin


#endif // MENUBARCOMPONENT_H_INCLUDED
