#ifndef LISTCELLIMPL_H_INCLUDED
#define LISTCELLIMPL_H_INCLUDED


#include "XULWin/Component.h"
#include "XULWin/AttributeController.h"
#include "XULWin/AttributeControllerPolicy.h"


namespace XULWin
{
    class ListCell;
    typedef AttributeInitializer<PassiveComponent, ListCell, LabelController_AsMember> ListCell_LabelController;

    class ListCell : public ListCell_LabelController
    {
    public:
        ListCell(Component * inParent, const AttributesMapping & inAttributesMapping);

        int calculateWidth(SizeConstraint inSizeConstraint) const;

        int calculateHeight(SizeConstraint inSizeConstraint) const;
    };

} // namespace XULWin


#endif // LISTCELLIMPL_H_INCLUDED
