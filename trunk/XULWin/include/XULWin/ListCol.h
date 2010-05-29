#ifndef LISTCOLIMPL_H_INCLUDED
#define LISTCOLIMPL_H_INCLUDED


#include "XULWin/Component.h"
#include "XULWin/Components.h"
#include <string>


namespace XULWin
{

    class ListCol : public DummyComponent
    {
    public:
        typedef DummyComponent Super;

        ListCol(Component * inParent, const AttributesMapping & inAttributesMapping);

        virtual bool init();

        int calculateWidth(SizeConstraint inSizeConstraint) const;

        int calculateHeight(SizeConstraint inSizeConstraint) const;
    };

} // namespace XULWin


#endif // LISTCOLIMPL_H_INCLUDED
