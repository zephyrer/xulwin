#ifndef LISTCOLSIMPL_H_INCLUDED
#define LISTCOLSIMPL_H_INCLUDED


#include "XULWin/Component.h"
#include "XULWin/Components.h"


namespace XULWin
{

    class ListCols : public DummyComponent
    {
    public:
        typedef DummyComponent Super;

        ListCols(Component * inParent, const AttributesMapping & inAttributesMapping);

        virtual bool init();

        bool initAttributeControllers();

        int calculateWidth(SizeConstraint inSizeConstraint) const;

        int calculateHeight(SizeConstraint inSizeConstraint) const;
    };

} // namespace XULWin


#endif // LISTCOLSIMPL_H_INCLUDED
