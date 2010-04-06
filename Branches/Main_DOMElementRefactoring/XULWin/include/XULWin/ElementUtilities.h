#ifndef ELEMENTUTILITIES_H_INCLUDED
#define ELEMENTUTILITIES_H_INCLUDED


#include <boost/function.hpp>


namespace Poco
{
    namespace XML
    {
        class Element;
        class Node;
        class NodeList;
    }
}


namespace XULWin
{

    Poco::XML::Element * Node2Element(Poco::XML::Node * inNode);

    // NodeIteration is a function object containing the loop code.
    typedef boost::function<void(Poco::XML::Node * )> NodeIteration;

    void ForEach(Poco::XML::NodeList * inNodes, const NodeIteration & inIteration);


} // namespace XULWin


#endif // ELEMENTUTILITIES_H_INCLUDED
