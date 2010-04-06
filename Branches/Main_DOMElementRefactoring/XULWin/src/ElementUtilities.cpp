#include "XULWin/ElementUtilities.h"
#include "Poco/DOM/Element.h"
#include "Poco/DOM/Node.h"
#include "Poco/DOM/NodeList.h"


namespace XULWin
{

    
    Poco::XML::Element * Node2Element(Poco::XML::Node * inNode) 
    {
        Poco::XML::Element * result = dynamic_cast<Poco::XML::Element *>(inNode);
        if (!result)
        {
            throw std::bad_cast("Downcast from Node toPoco::XML::Element failed.");
        }
        return result;
    }

    
    void ForEach(Poco::XML::NodeList * inNodes, const NodeIteration & inIteration)
    {
        if (inNodes->length() == 0)
        {
            return;
        }

        Poco::XML::Node * currentNode = inNodes->item(0);
        while (currentNode)
        {
            inIteration(currentNode);
            currentNode = currentNode->nextSibling();
        }
    }

} // namespace XULWin
