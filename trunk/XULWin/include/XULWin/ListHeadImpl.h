#ifndef LISTHEADIMPL_H_INCLUDED
#define LISTHEADIMPL_H_INCLUDED


#include "XULWin/Component.h"


namespace XULWin
{

    class ListHeadImpl : public PassiveComponent
    {
    public:
        typedef PassiveComponent Super;

        ListHeadImpl(Component * inParent, const AttributesMapping & inAttributesMapping);

        virtual bool initImpl();

        bool initAttributeControllers();

        int calculateWidth(SizeConstraint inSizeConstraint) const;

        int calculateHeight(SizeConstraint inSizeConstraint) const;

        virtual void onChildAdded(Component * inChild);
    };

} // namespace XULWin


#endif // LISTHEADIMPL_H_INCLUDED
