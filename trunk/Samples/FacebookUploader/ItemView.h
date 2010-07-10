#ifndef ITEMVIEW_H_INCLUDED
#define ITEMVIEW_H_INCLUDED


#include "Item.h"
#include "XULWin/Gdiplus.h"
#include "XULWin/GdiplusUtils.h"
#include "XULWin/WinUtils.h"
#include <string>
#include <vector>
#include <boost/noncopyable.hpp>
#include <boost/scoped_ptr.hpp>


namespace XULWin
{

    /**
     * ItemView is the abstract base class that contains view-specific data that belongs to an Item.
     */
    class ItemView : private boost::noncopyable
    {
    public:
        // Does not take ownership. Ownership belongs to the model.
        ItemView(Item * inItem);

        const Item * item() const;

        Item * item();

        Gdiplus::Bitmap * getThumbnail() const;
        
    private:
        Item * mItem;
        mutable boost::scoped_ptr<Gdiplus::Bitmap> mThumbnail;
    };

    typedef boost::shared_ptr<ItemView> ItemViewPtr;
    typedef std::set<ItemViewPtr> ItemViews;

    bool operator < (const ItemView & lhs, const ItemView & rhs);

    bool operator < (const ItemViewPtr & lhs, const ItemViewPtr & rhs);

}


#endif // ITEMVIEW_H_INCLUDED
