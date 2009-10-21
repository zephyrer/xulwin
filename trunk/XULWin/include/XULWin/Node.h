#ifndef NODE_H_INCLUDED
#define NODE_H_INCLUDED


#include <set>
#include <vector>
#include <boost/shared_ptr.hpp>


namespace XULWin
{


    template<class ValueType>
    struct ContainerPolicy_Set
    {
        typedef std::set<ValueType> Container;
        static void insert(Container & ioContainer, const ValueType & inValue)
        {
            ioContainer.insert(inValue);
        }
    };


    template<class ValueType>
    struct ContainerPolicy_Vector
    {
        typedef std::vector<ValueType> Container;
        static void insert(Container & ioContainer, const ValueType & inValue)
        {
            ioContainer.push_back(inValue);
        }
    };

    template <class PointeeType>
    struct PointerPolicy_Normal
    {
        typedef PointeeType* PointerType;

        static PointeeType* getRaw(PointerType p)
        {
            return p;
        }
    };

    template <class PointeeType>
    struct PointerPolicy_Shared
    {
        typedef boost::shared_ptr<PointeeType> PointerType;

        static PointeeType* getRaw(PointerType p)
        {
            return p.get();
        }
    };


    template <class DataType, template <class> class ContainerPolicy, template <class> class PointerPolicy>
    class GenericNode
    {
    public:    
        GenericNode() { }

        GenericNode(const DataType & inData) : mData(inData) { }

        typedef GenericNode<DataType, ContainerPolicy, PointerPolicy> This;
        typedef typename PointerPolicy<This>::PointerType ChildPtr;
        typedef typename ContainerPolicy<ChildPtr>::Container Container;

        typedef typename Container::iterator iterator;
        typedef typename Container::const_iterator const_iterator;

        iterator begin() { return mChildren.begin(); }
        iterator end() { return mChildren.end(); }

        const_iterator begin() const { return mChildren.begin(); }
        const_iterator end() const { return mChildren.end(); }

        void addChild(This * inItem)
        { 
            ChildPtr item(inItem);
            ContainerPolicy<ChildPtr>::insert(mChildren, item);
        }

        size_t size() const { return mChildren.size(); }

        bool empty() const { return mChildren.empty(); }

        const DataType & data() const
        { return mData; }

        void setData(const DataType & inData)
        { mData = inData; }

    private:
        DataType mData;
        Container mChildren;
    };



} // namespace XULWin


#endif // NODE_H_INCLUDED
