#ifndef NEWELEMENTSIMPL_H_INCLUDED
#define NEWELEMENTSIMPL_H_INCLUDED


#include "XULWin/Element.h"


namespace XULWin
{

    class NewElementS : public Passive
    {
    public:
        typedef Passive Super;

        NewElementS(Element * inParent, const AttributesMapping & inAttributesMapping);

        virtual bool init();

        bool initAttributeControllers();

        int calculateWidth(SizeConstraint inSizeConstraint) const;

        int calculateHeight(SizeConstraint inSizeConstraint) const;
    };

} // namespace XULWin


#endif // NEWELEMENTSIMPL_H_INCLUDED
