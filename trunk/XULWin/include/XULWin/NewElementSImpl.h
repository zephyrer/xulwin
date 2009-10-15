#ifndef NEWELEMENTSIMPL_H_INCLUDED
#define NEWELEMENTSIMPL_H_INCLUDED


#include "XULWin/ElementImpl.h"


namespace XULWin
{

    class NewElementSImpl : public PassiveComponent
    {
    public:
        typedef PassiveComponent Super;

        NewElementSImpl(ElementImpl * inParent, const AttributesMapping & inAttributesMapping);

        virtual bool initImpl();

        bool initAttributeControllers();

        int calculateWidth(SizeConstraint inSizeConstraint) const;

        int calculateHeight(SizeConstraint inSizeConstraint) const;
    };

} // namespace XULWin


#endif // NEWELEMENTSIMPL_H_INCLUDED
