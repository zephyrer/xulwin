#ifndef FACEBOOKUPLOADERMODEL_H_INCLUDED
#define FACEBOOKUPLOADERMODEL_H_INCLUDED


#include "Item.h"


namespace XULWin
{

    class FacebookUploaderModel
    {
    public:
        FacebookUploaderModel();

        Items & getItems();

        const Items & getItems() const;
        
    private:
        Items mItems;
    };


}


#endif // FACEBOOKUPLOADERMODEL_H_INCLUDED
