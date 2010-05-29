#ifndef LISTCOLSIMPL_H_INCLUDED
#define LISTCOLSIMPL_H_INCLUDED


#include "XULWin/Component.h"
#include "XULWin/ImaginaryComponent.h"


namespace XULWin
{

    class ListCols : public ImaginaryComponent
    {
    public:
        typedef ImaginaryComponent Super;

        ListCols(Component * inParent, const AttributesMapping & inAttributesMapping);

        virtual bool init();

        bool initAttributeControllers();

        int calculateWidth(SizeConstraint inSizeConstraint) const;

        int calculateHeight(SizeConstraint inSizeConstraint) const;
    };

} // namespace XULWin


#endif // LISTCOLSIMPL_H_INCLUDED
