#include "XULWin/XULOverlayParser.h"
#include "XULWin/ElementFactory.h"
#include "XULWin/ChromeURL.h"
#include "XULWin/Defaults.h"
#include "XULWin/ErrorReporter.h"
#include "Poco/SAX/Attributes.h"
#include "Poco/SAX/EntityResolverImpl.h"


namespace XULWin
{


    XULOverlayParser::XULOverlayParser(Element * inOverlayElement) :
        mOverlayRoot(inOverlayElement)
    {
    }


    Element * XULOverlayParser::getCurrentParentElement()
    {
        if (mStack.empty())
        {
            // The root element naturally has no parent.
            return 0;
        }
        if (mStack.size() == 1)
        {
            // The <overlay> element is not included in the element hierachy.
            return 0;
        }
        else if (mStack.size() == 2)
        {
            // The second element on the stack is a reference to the element that gets overlaid in
            // the original document. Because this element already exists, we don't create it. And
            // for its children we give the existing element as parent.
            return mOverlayRoot;
        }
        else
        {
            // We can revert to default behavior once we get past the overlay initialization
            // elements.
            return mStack.top();
        }
    }


    bool XULOverlayParser::createElement(const std::string & inLocalName,
                                         Element * inParent,
                                         const AttributesMapping & inAttributes,
                                         ElementPtr & outElement)
    {
        if (mStack.size() < 2)
        {
            // The <overlay> element and its dummy child must not be created.
            // We return "true" here to indicate that we do want to continue
            // parsing child elements.
            return true;
        }
        else
        {
            // Create the child element. If the factory returns a nil element, then return
            // false to indicate that we failed to parse this element, and that all its child
            // elements have to be ignored as well.
            outElement = ElementFactory::Instance().createElement(inLocalName, inParent, inAttributes);
            return outElement.get() != 0;
        }
    }


    void XULOverlayParser::pushStack(ElementPtr inElement)
    {
        mStack.push(inElement.get());
    }


    void XULOverlayParser::popStack()
    {
        if (mStack.size() > 2)
        {
            mStack.top()->init();
            mStack.pop();
        }
    }


} // namespace XULWin
