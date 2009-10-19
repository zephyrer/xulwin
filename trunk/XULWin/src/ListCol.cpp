#include "XULWin/ListCol.h"
#include "XULWin/ListCellElement.h"
#include "XULWin/ListCell.h"
#include "XULWin/ListItemElement.h"
#include "XULWin/ListItem.h"
#include "XULWin/Decorator.h"


namespace XULWin
{

    ListColImpl::ListColImpl(Component * inParent, const AttributesMapping & inAttributesMapping) :
        Super(inParent, inAttributesMapping)
    {
    }
        
        
    int ListColImpl::calculateWidth(SizeConstraint inSizeConstraint) const
    {  
        int res = 1;
        // XUL Hierarchy:
        // listbox/listcols/listcol
        // listbox/listitem/listcell
        std::vector<ListItemElement*> listItems;
        parent()->parent()->el()->getElementsByType<ListItemElement>(listItems);
        const int colIdx = getIndex();
        for (size_t idx = 0; idx != listItems.size(); ++idx)
        {
            if (ListCellImpl * cell = listItems[idx]->component()->getChild(colIdx)->downcast<ListCellImpl>())
            {
                res = std::max<int>(res, cell->calculateWidth(inSizeConstraint));
            }
        }
        return res;
    }

    
    int ListColImpl::calculateHeight(SizeConstraint inSizeConstraint) const
    {
        return 1;
    }


    bool ListColImpl::initComponent()
    {
        return Super::initComponent();
    }
    
} // namespace XULWin
