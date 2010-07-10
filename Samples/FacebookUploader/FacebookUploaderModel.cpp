#include "FacebookUploaderModel.h"


namespace XULWin
{

    
    FacebookUploaderModel::FacebookUploaderModel()
    {
    }

    
    Items & FacebookUploaderModel::getItems()
    {
        return mItems;
    }


    const Items & FacebookUploaderModel::getItems() const
    {
        return mItems;
    }


    void FacebookUploaderModel::addItem(ItemPtr inItem)
    {
        if (mItems.find(inItem) == mItems.end())
        {
            mItems.insert(inItem);
        }
    }


} // namespace XULWin
