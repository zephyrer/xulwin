#include "XULWin/ListCol.h"
#include "XULWin/Element.h"
#include "XULWin/XMLListCell.h"
#include "XULWin/ListCell.h"
#include "XULWin/ListItem.h"
#include "XULWin/Decorator.h"


namespace XULWin
{

    ListCol::ListCol(Component * inParent, const AttributesMapping & inAttr) :
        Super(inParent, inAttr)
    {
    }


    int ListCol::calculateWidth(SizeConstraint inSizeConstraint) const
    {
        int res = 1;
        // XUL Hierarchy:
        // listbox/listcols/listcol
        // listbox/listitem/listcell
        std::vector<XMLListItem *> listItems;
        parent()->parent()->el()->getElementsByType<XMLListItem>(listItems);
        const int colIdx = getIndex();
        for (size_t idx = 0; idx != listItems.size(); ++idx)
        {
            if (ListCell * cell = listItems[idx]->component()->getChild(colIdx)->downcast<ListCell>())
            {
                res = std::max<int>(res, cell->calculateWidth(inSizeConstraint));
            }
        }
        return res;
    }


    int ListCol::calculateHeight(SizeConstraint inSizeConstraint) const
    {
        return 1;
    }


    bool ListCol::init()
    {
        return Super::init();
    }

} // namespace XULWin
