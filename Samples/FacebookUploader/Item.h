#ifndef ITEM_H_INCLUDED
#define ITEM_H_INCLUDED


#include <set>
#include <string>
#include <vector>
#include <boost/shared_ptr.hpp>


namespace XULWin
{


    /**
     * Item represents something that can be uploaded to Facebook.
     * For example: picture, video, ...?
     */
    class Item
    {
    public:
        Item(const std::string & inPath);

        const std::string & path() const;

        // Does not take ownership.
        void setUserData(void * inUserData);

        void * getUserData() const;

        template<class T>
        T * getTypedUserData() const
        {
            return static_cast<T*>(getUserData());
        }
        
    private:
        void * mUserData;
        std::string mPath;
    };

    typedef boost::shared_ptr<Item> ItemPtr;
    typedef std::set<ItemPtr> Items;

    bool operator< (const Item & lhs, const Item & rhs);
    bool operator< (const ItemPtr lhs, const ItemPtr & rhs);

}


#endif // ITEM_H_INCLUDED
