#include "XULWin/XULParser.h"
#include "XULWin/ElementFactory.h"
#include "XULWin/ChromeURL.h"
#include "XULWin/Defaults.h"
#include "XULWin/ErrorReporter.h"
#include "Poco/SAX/Attributes.h"
#include "Poco/SAX/EntityResolverImpl.h"


namespace XULWin
{

    AbstractXULParser::AbstractXULParser() :
        mIgnores(0),
        mLanguage("en"),
        mExtraContentHandler(0)
    {
        setFeature(FEATURE_EXTERNAL_GENERAL_ENTITIES, true);
        setFeature(FEATURE_EXTERNAL_PARAMETER_ENTITIES, true);

        setContentHandler(this);
        setEntityResolver(this);
    }


    ElementPtr AbstractXULParser::rootElement() const
    {
        return mRootElement;
    }


    void AbstractXULParser::setExtraContentHander(Poco::XML::ContentHandler * inContentHandler)
    {
        mExtraContentHandler = inContentHandler;
    }


    void AbstractXULParser::setDocumentLocator(const Poco::XML::Locator * inLocator)
    {
        mLocator = inLocator;
        if (mExtraContentHandler)
        {
            mExtraContentHandler->setDocumentLocator(inLocator);
        }
        
    }


    void AbstractXULParser::startDocument()
    {
        assert(mIgnores == 0);
        if (mExtraContentHandler)
        {
            mExtraContentHandler->startDocument();
        }
    }


    void AbstractXULParser::endDocument()
    {
        assert(mIgnores == 0);
        if (mExtraContentHandler)
        {
            mExtraContentHandler->endDocument();
        }
    }


    void AbstractXULParser::getAttributes(const Poco::XML::Attributes & inXMLAttributes,
                                          AttributesMapping & outXULAttributes)
    {
        for (int idx = 0; idx != inXMLAttributes.getLength(); ++idx)
        {
            const Poco::XML::XMLString & name = inXMLAttributes.getLocalName(idx);
            const Poco::XML::XMLString & value = inXMLAttributes.getValue(idx);
            outXULAttributes[name] = value;
        }
    }


    void AbstractXULParser::startElement(const Poco::XML::XMLString & uri,
                                         const Poco::XML::XMLString & localName,
                                         const Poco::XML::XMLString & qname,
                                         const Poco::XML::Attributes & attributes)
    {
        try
        {
            if (mIgnores > 0)
            {
                mIgnores++;
                return;
            }

            Element * parent = getCurrentParentElement();
            AttributesMapping attr;
            getAttributes(attributes, attr);
            ElementPtr element;
            if (createElement(localName, parent, attr, element))
            {
                pushStack(element);
            }
            else
            {
                mIgnores++;
                ReportError("Element is null and will be ignored.");
                return;
            }
        }
        catch (const Poco::Exception & inExc)
        {
            ReportError(inExc.displayText());
        }

        if (mExtraContentHandler)
        {
            mExtraContentHandler->startElement(uri, localName, qname, attributes);
        }
    }


    void AbstractXULParser::endElement(const Poco::XML::XMLString & uri,
                                       const Poco::XML::XMLString & localName,
                                       const Poco::XML::XMLString & qname)
    {
        if (mIgnores > 0)
        {
            mIgnores--;
            return;
        }
        popStack();

        if (mExtraContentHandler)
        {
            mExtraContentHandler->endElement(uri, localName, qname);
        }
    }


    void AbstractXULParser::characters(const Poco::XML::XMLChar ch[], int start, int length)
    {
        if (!mStack.empty() && mStack.top())
        {
            std::string innerText = std::string(ch + start, length);
            mStack.top()->setInnerText(mStack.top()->innerText() + innerText);
        }

        if (mExtraContentHandler)
        {
            mExtraContentHandler->characters(ch, start, length);
        }
    }


    Poco::XML::InputSource * AbstractXULParser::resolveEntity(const Poco::XML::XMLString * publicId, const Poco::XML::XMLString & systemId)
    {
        try
        {
            size_t idx = systemId.find("chrome://");
            if (idx != std::string::npos)
            {
                ChromeURL url(systemId);
                std::string path = url.convertToLocalPath();
                Poco::XML::EntityResolverImpl entityResolverImpl;
                return entityResolverImpl.resolveEntity(publicId, path);
            }
        }
        catch (const Poco::Exception & inExc)
        {
            ReportError(inExc.displayText());
        }
        return 0;
    }


    void AbstractXULParser::releaseInputSource(Poco::XML::InputSource * pSource)
    {
        Poco::XML::EntityResolverImpl entityResolverImpl;
        return entityResolverImpl.releaseInputSource(pSource);
    }


    bool XULParser::createElement(const std::string & inLocalName,
                                  Element * inParent,
                                  const AttributesMapping & inAttributes,
                                  ElementPtr & outElement)
    {
        outElement = ElementFactory::Instance().createElement(inLocalName, inParent, inAttributes);
        return outElement.get() != 0;
    }


    Element * XULParser::getCurrentParentElement()
    {
        Element * parent(0);
        if (!mStack.empty())
        {
            parent = mStack.top();
        }
        return parent;
    }


    void XULParser::pushStack(ElementPtr inElement)
    {
        if (inElement)
        {
            if (mStack.empty())
            {
                assert(!mRootElement);
                mRootElement = inElement;
            }
            mStack.push(inElement.get());
        }
    }


    void XULParser::popStack()
    {
        assert(!mStack.empty());
        if (!mStack.empty())
        {
            mStack.top()->init();
            mStack.pop();
        }
    }


} // namespace XULWin
