#ifndef LISTCOLIMPL_H_INCLUDED
#define LISTCOLIMPL_H_INCLUDED


#include "XULWin/Component.h"
#include <string>


namespace XULWin
{

    class ListColImpl : public PassiveComponent
    {
    public:
        typedef PassiveComponent Super;

        ListColImpl(Component * inParent, const AttributesMapping & inAttributesMapping);

        virtual bool initComponent();

        int calculateWidth(SizeConstraint inSizeConstraint) const;

        int calculateHeight(SizeConstraint inSizeConstraint) const;
    };

} // namespace XULWin


#endif // LISTCOLIMPL_H_INCLUDED
