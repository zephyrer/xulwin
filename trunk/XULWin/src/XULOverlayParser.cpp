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
        mOverlayElement(inOverlayElement)
    {
    }
    
    
    Element * XULOverlayParser::getCurrentParentElement()
    {
        if (mStack.empty())
        {
            // OverlayElement has NULL parent.
            return 0;
        }
        else if (mStack.size() == 1)
        {
            // Dummy element takes overlay as parent.
            return mStack.top();
        }
        else if (mStack.size() == 2)
        {
            // First real element gets overlay element as top.
            return mOverlayElement;
        }
        else
        {
            return mStack.top();
        }
    }


    bool XULOverlayParser::createElement(const std::string & inLocalName,
                                         Element * inParent,
                                         const AttributesMapping & inAttributes,
                                         ElementPtr & outElement)
    {
        if (mStack.size() >= 2)
        {
            outElement = ElementFactory::Instance().createElement(inLocalName, inParent, inAttributes);
            return outElement.get() != 0;
        }
        // This element is not created, but we don't want to skip its children.
        return true;
    }


    void XULOverlayParser::pushStack(ElementPtr inElement)
    {       
        if (mStack.size() == 2)
        {
            assert(!mRootElement);
            mRootElement = inElement;
        }
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
