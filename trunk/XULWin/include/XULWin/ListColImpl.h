#ifndef LISTCOLIMPL_H_INCLUDED
#define LISTCOLIMPL_H_INCLUDED


#include "XULWin/ElementImpl.h"


namespace XULWin
{

    class ListColImpl : public PassiveComponent
    {
    public:
        typedef PassiveComponent Super;

        ListColImpl(ElementImpl * inParent, const AttributesMapping & inAttributesMapping);

        virtual bool initImpl();

        int calculateWidth(SizeConstraint inSizeConstraint) const;

        int calculateHeight(SizeConstraint inSizeConstraint) const;
    };

} // namespace XULWin


#endif // LISTCOLIMPL_H_INCLUDED
