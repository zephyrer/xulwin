#ifndef LISTCOLIMPL_H_INCLUDED
#define LISTCOLIMPL_H_INCLUDED


#include "XULWin/PhonyComponent.h"
#include <string>


namespace XULWin
{

    class ListCol : public PhonyComponent
    {
    public:
        typedef PhonyComponent Super;

        ListCol(Component * inParent, const AttributesMapping & inAttr);

        virtual bool init();

        int calculateWidth(SizeConstraint inSizeConstraint) const;

        int calculateHeight(SizeConstraint inSizeConstraint) const;
    };

} // namespace XULWin


#endif // LISTCOLIMPL_H_INCLUDED
