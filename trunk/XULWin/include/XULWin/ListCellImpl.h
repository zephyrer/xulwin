#ifndef LISTCELLIMPL_H_INCLUDED
#define LISTCELLIMPL_H_INCLUDED


#include "XULWin/ElementImpl.h"


namespace XULWin
{

    class ListCellImpl : public PassiveComponent
    {
    public:
        typedef PassiveComponent Super;

        ListCellImpl(ElementImpl * inParent, const AttributesMapping & inAttributesMapping);

        virtual bool initImpl();

        bool initAttributeControllers();

        int calculateWidth(SizeConstraint inSizeConstraint) const;

        int calculateHeight(SizeConstraint inSizeConstraint) const;
    };

} // namespace XULWin


#endif // LISTCELLIMPL_H_INCLUDED
