#ifndef LISTCOLIMPL_H_INCLUDED
#define LISTCOLIMPL_H_INCLUDED


#include "XULWin/ImaginaryComponent.h"
#include <string>


namespace XULWin
{

    class ListCol : public ImaginaryComponent
    {
    public:
        typedef ImaginaryComponent Super;

        ListCol(Component * inParent, const AttributesMapping & inAttributesMapping);

        virtual bool init();

        int calculateWidth(SizeConstraint inSizeConstraint) const;

        int calculateHeight(SizeConstraint inSizeConstraint) const;
    };

} // namespace XULWin


#endif // LISTCOLIMPL_H_INCLUDED
