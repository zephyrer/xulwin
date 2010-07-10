#include "ItemView.h"
#include "XULWin/Unicode.h"


namespace XULWin
{
    
    
    ItemView::ItemView(Item * inItem) :
        mItem(inItem)
    {
        if (!mItem)
        {
            throw std::runtime_error("Item passed to ItemView must not be null.");
        }
    }
        
        
    const Item * ItemView::item() const
    {
        return mItem;
    }


    Item * ItemView::item()
    {
        return mItem;
    }


    Gdiplus::Bitmap * ItemView::getThumbnail() const
    {
        if (!mThumbnail)
        {
            std::wstring utf16Path = ToUTF16(mItem->path());
            mThumbnail.reset(new Gdiplus::Bitmap(utf16Path.c_str()));
            if (mThumbnail->GetLastStatus() != Gdiplus::Ok)
            {
                throw std::runtime_error("Failed to create a thumbnail for " + mItem->path());
            }
        }
        return mThumbnail.get();
    }


    bool operator < (const ItemView & lhs, const ItemView & rhs)
    {
        return lhs.item() < rhs.item();
    }


} // namespace XULWin
