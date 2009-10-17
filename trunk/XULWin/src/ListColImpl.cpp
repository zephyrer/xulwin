#include "XULWin/ListColImpl.h"
#include "XULWin/ListCell.h"
#include "XULWin/ListCellImpl.h"
#include "XULWin/ListItem.h"
#include "XULWin/ListItemImpl.h"
#include "XULWin/Decorator.h"


namespace XULWin
{

    ListColImpl::ListColImpl(ElementImpl * inParent, const AttributesMapping & inAttributesMapping) :
        Super(inParent, inAttributesMapping)
    {
    }
        
        
    int ListColImpl::calculateWidth(SizeConstraint inSizeConstraint) const
    {  
        int res = 1;
        // XUL Hierarchy:
        // listbox/listcols/listcol
        // listbox/listitem/listcell
        std::vector<ListItem*> listItems;
        parent()->parent()->el()->getElementsByType<ListItem>(listItems);
        const int colIdx = getIndex();
        for (size_t idx = 0; idx != listItems.size(); ++idx)
        {
            if (ListCellImpl * cell = listItems[idx]->impl()->getChild(colIdx)->downcast<ListCellImpl>())
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


    bool ListColImpl::initImpl()
    {
        return Super::initImpl();
    }
    
} // namespace XULWin
