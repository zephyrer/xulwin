#ifndef LISTCOLSIMPL_H_INCLUDED
#define LISTCOLSIMPL_H_INCLUDED


#include "XULWin/ElementImpl.h"


namespace XULWin
{

    class ListColsImpl : public PassiveComponent
    {
    public:
        typedef PassiveComponent Super;

        ListColsImpl(ElementImpl * inParent, const AttributesMapping & inAttributesMapping);

        virtual bool initImpl();

        bool initAttributeControllers();

        int calculateWidth(SizeConstraint inSizeConstraint) const;

        int calculateHeight(SizeConstraint inSizeConstraint) const;
    };

} // namespace XULWin


#endif // LISTCOLSIMPL_H_INCLUDED
