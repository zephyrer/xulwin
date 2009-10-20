#ifndef LISTHEADIMPL_H_INCLUDED
#define LISTHEADIMPL_H_INCLUDED


#include "XULWin/Component.h"


namespace XULWin
{

    class ListHead : public PassiveComponent
    {
    public:
        typedef PassiveComponent Super;

        ListHead(Component * inParent, const AttributesMapping & inAttributesMapping);

        virtual bool initComponent();

        bool initAttributeControllers();

        int calculateWidth(SizeConstraint inSizeConstraint) const;

        int calculateHeight(SizeConstraint inSizeConstraint) const;

        virtual void onChildAdded(Component * inChild);
    };

} // namespace XULWin


#endif // LISTHEADIMPL_H_INCLUDED
