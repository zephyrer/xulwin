#include "Item.h"

namespace XULWin
{
    
    
    Item::Item(const std::string & inPath, void * inUserData) :
        mPath(inPath),
        mUserData(inUserData)
    {
    }


    const std::string & Item::path() const
    {
        return mPath;
    }


    void Item::setUserData(void * inUserData)
    {
        mUserData = inUserData;
    }


    void * Item::getUserData() const
    {
        return mUserData;
    }


    bool operator< (const Item & lhs, const Item & rhs)
    {
        return lhs.path() < rhs.path();
    }
    
    
    bool operator< (const ItemPtr lhs, const ItemPtr & rhs)
    {
        if (!lhs || !rhs)
        {
            throw std::logic_error("Operator < for ItemPtr failed because of a null shared_ptr object.");
        }
        
        return *lhs < *rhs;
    }


} // namespace XULWin
