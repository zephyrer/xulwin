#ifndef LISTHEADIMPL_H_INCLUDED
#define LISTHEADIMPL_H_INCLUDED


#include "XULWin/Component.h"
#include "XULWin/Components.h"


namespace XULWin
{

    class ListHead : public VirtualComponent
    {
    public:
        typedef VirtualComponent Super;

        ListHead(Component * inParent, const AttributesMapping & inAttributesMapping);

        virtual bool init();

        bool initAttributeControllers();

        int calculateWidth(SizeConstraint inSizeConstraint) const;

        int calculateHeight(SizeConstraint inSizeConstraint) const;

        virtual void onChildAdded(Component * inChild);
    };

} // namespace XULWin


#endif // LISTHEADIMPL_H_INCLUDED
