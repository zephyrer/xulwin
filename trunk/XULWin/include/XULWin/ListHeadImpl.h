#ifndef LISTHEADIMPL_H_INCLUDED
#define LISTHEADIMPL_H_INCLUDED


#include "XULWin/ElementImpl.h"


namespace XULWin
{

    class ListHeadImpl : public PassiveComponent
    {
    public:
        typedef PassiveComponent Super;

        ListHeadImpl(ElementImpl * inParent, const AttributesMapping & inAttributesMapping);

        virtual bool initImpl();

        bool initAttributeControllers();

        int calculateWidth(SizeConstraint inSizeConstraint) const;

        int calculateHeight(SizeConstraint inSizeConstraint) const;

        virtual void onChildAdded(ElementImpl * inChild);
    };

} // namespace XULWin


#endif // LISTHEADIMPL_H_INCLUDED
