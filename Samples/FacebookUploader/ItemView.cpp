#include "ItemView.h"

namespace XULWin
{
    
    
    ItemView::ItemView(Item * inItem) :
        mItem(inItem)
    {
    }
        
        
    const Item * ItemView::item() const
    {
        return mItem;
    }


    Item * ItemView::item()
    {
        return mItem;
    }


    bool operator < (const ItemView & lhs, const ItemView & rhs)
    {
        return lhs.item() < rhs.item();
    }


} // namespace XULWin
