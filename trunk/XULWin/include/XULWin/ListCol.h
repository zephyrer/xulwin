#ifndef LISTCOLIMPL_H_INCLUDED
#define LISTCOLIMPL_H_INCLUDED


#include "XULWin/Component.h"
#include "XULWin/Components.h"
#include <string>


namespace XULWin
{

    class ListCol : public PassiveComponent
    {
    public:
        typedef PassiveComponent Super;

        ListCol(Component * inParent, const AttributesMapping & inAttributesMapping);

        virtual bool init();

        int calculateWidth(SizeConstraint inSizeConstraint) const;

        int calculateHeight(SizeConstraint inSizeConstraint) const;
    };

} // namespace XULWin


#endif // LISTCOLIMPL_H_INCLUDED
