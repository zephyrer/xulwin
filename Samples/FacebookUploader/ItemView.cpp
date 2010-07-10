#include "ItemView.h"

namespace XULWin
{
    
    
    ItemView::ItemView(ItemPtr inItem) :
        mItem(inItem)
    {
    }
        
        
    const ItemPtr ItemView::item() const
    {
        return mItem;
    }


    ItemPtr ItemView::item()
    {
        return mItem;
    }


    bool operator < (const ItemView & lhs, const ItemView & rhs)
    {
        return lhs.item() < rhs.item();
    }


} // namespace XULWin
