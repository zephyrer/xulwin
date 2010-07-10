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


} // namespace XULWin
