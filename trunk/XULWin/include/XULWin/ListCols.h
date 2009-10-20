#ifndef LISTCOLSIMPL_H_INCLUDED
#define LISTCOLSIMPL_H_INCLUDED


#include "XULWin/Component.h"


namespace XULWin
{

    class ListCols : public PassiveComponent
    {
    public:
        typedef PassiveComponent Super;

        ListCols(Component * inParent, const AttributesMapping & inAttributesMapping);

        virtual bool initComponent();

        bool initAttributeControllers();

        int calculateWidth(SizeConstraint inSizeConstraint) const;

        int calculateHeight(SizeConstraint inSizeConstraint) const;
    };

} // namespace XULWin


#endif // LISTCOLSIMPL_H_INCLUDED
