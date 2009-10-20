#ifndef NODE_H_INCLUDED
#define NODE_H_INCLUDED


#include <vector>
#include <boost/shared_ptr.hpp>


namespace XULWin
{


    template <class PointeeType>
    struct NormalPointerPolicy
    {
        typedef PointeeType* PointerType;

        static PointeeType* getRaw(PointerType p)
        {
            return p;
        }
    };

    template <class PointeeType>
    struct SharedPointerPolicy
    {
        typedef boost::shared_ptr<PointeeType> PointerType;

        static PointeeType* getRaw(PointerType p)
        {
            return p.get();
        }
    };


    template <class DataType, template <class> class PointerPolicy>
    class GenericNode
    {
    public:    
        GenericNode() { }

        GenericNode(const DataType & inData) : mData(inData) { }

        typedef GenericNode<DataType, PointerPolicy> This;

        typedef typename PointerPolicy<This>::PointerType ChildPtr;
        
        const This * getChild(size_t idx) const
        { return PointerPolicy<This>::getRaw(mChildren[idx]); }

        This * getChild(size_t idx)
        { return PointerPolicy<This>::getRaw(mChildren[idx]); }

        void addChild(This * inItem)
        { 
            ChildPtr item(inItem);
            mChildren.push_back(item);
        }

        size_t getChildCount() const
        { return mChildren.size(); }

        const DataType & data() const
        { return mData; }

        void setData(const DataType & inData)
        { mData = inData; }

    private:
        DataType mData;
        std::vector<ChildPtr> mChildren;
    };


} // namespace XULWin


#endif // NODE_H_INCLUDED
