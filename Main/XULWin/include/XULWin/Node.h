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

        static size_t size(const Container & ioContainer)
        {
            return ioContainer.size();
        }

        static bool empty(const Container & ioContainer)
        {
            return ioContainer.empty();
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

        static size_t size(const Container & ioContainer)
        {
            return ioContainer.size();
        }

        static bool empty(const Container & ioContainer)
        {
            return ioContainer.empty();
        }
    };


    template <class PointeeType>
    struct PointerPolicy_Normal_NoOwnership
    {
        typedef PointeeType * PointerType;

        static PointeeType * getRaw(PointerType p)
        {
            return p;
        }

        static void destroy(PointerType p) { }
    };


    template <class PointeeType>
    struct PointerPolicy_Normal_WithOwnership
    {
        typedef PointeeType * PointerType;

        static PointeeType * getRaw(PointerType p)
        {
            return p;
        }

        static void destroy(PointerType p)
        {
            delete p;
        }
    };


    template <class PointeeType>
    struct PointerPolicy_Shared
    {
        typedef boost::shared_ptr<PointeeType> PointerType;

        static PointeeType * getRaw(PointerType p)
        {
            return p.get();
        }

        static void destroy(PointerType p) { }
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

        ~GenericNode()
        {
            const_iterator it = this->begin(), endIt = this->end();
            for (; it != endIt; ++it)
            {
                PointerPolicy<This>::destroy(*it);
            }
        }

        iterator begin()
        {
            return mChildren.begin();
        }
        iterator end()
        {
            return mChildren.end();
        }

        const_iterator begin() const
        {
            return mChildren.begin();
        }
        const_iterator end() const
        {
            return mChildren.end();
        }

        /**
         * Takes ownership.
         */
        void addChild(This * inItem)
        {
            ChildPtr item(inItem);
            ContainerPolicy<ChildPtr>::insert(mChildren, item);
        }

        size_t size() const
        {
            return ContainerPolicy<ChildPtr>::size(mChildren);
        }

        bool empty() const
        {
            return ContainerPolicy<ChildPtr>::empty(mChildren);
        }

        const DataType & data() const
        {
            return mData;
        }

        void setData(const DataType & inData)
        {
            mData = inData;
        }

    private:
        DataType mData;
        Container mChildren;
    };



} // namespace XULWin


#endif // NODE_H_INCLUDED
