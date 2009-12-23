#include "XULWin/Parser.h"
#include "XULWin/ElementFactory.h"
#include "XULWin/ChromeURL.h"
#include "XULWin/Defaults.h"
#include "XULWin/ErrorReporter.h"
#include "Poco/SAX/Attributes.h"
#include "Poco/SAX/EntityResolverImpl.h"


namespace XULWin
{

    Parser::Parser() :
        mIgnores(0),
        mLanguage("en"),
        mIsOverlay(false),
        mOverlayWrappers(0)
    {
        setFeature(FEATURE_EXTERNAL_GENERAL_ENTITIES, true);
        setFeature(FEATURE_EXTERNAL_PARAMETER_ENTITIES, true);
        
        setContentHandler(this);
        setEntityResolver(this);
    }


    Parser::Parser(Element * inOverlayRoot) :
        mIgnores(0),
        mLanguage("en"),
        mIsOverlay(true),
        mOverlayWrappers(0)
    {
        setFeature(FEATURE_EXTERNAL_GENERAL_ENTITIES, true);
        setFeature(FEATURE_EXTERNAL_PARAMETER_ENTITIES, true);
        
        setContentHandler(this);
        setEntityResolver(this);

        mStack.push(inOverlayRoot);
    }
    
    
    ElementPtr Parser::rootElement() const
    {
        return mRootElement;
    }


    void Parser::setDocumentLocator(const Poco::XML::Locator * inLocator)
    {
        mLocator = inLocator;
    }


    void Parser::startDocument()
    {
        assert(mIgnores == 0);
    }


    void Parser::endDocument()
    {
        assert(mIgnores == 0);
    }


    void Parser::startElement(const Poco::XML::XMLString& uri,
                              const Poco::XML::XMLString& localName,
                              const Poco::XML::XMLString& qname,
                              const Poco::XML::Attributes& attributes)
    {
        try
        {
            if (mIsOverlay && mOverlayWrappers < 2)
            {
                mOverlayWrappers++;
                return;
            }

            if (mIgnores > 0)
            {
                mIgnores++;
                return;
            }

            //
            // Get parent
            //
            Element * parent(0);
            if (!mStack.empty())
            {
                parent = mStack.top();
            }

            //
            // Get attributes
            //
            AttributesMapping attr;
            for (int idx = 0; idx != attributes.getLength(); ++idx)
            {
                const Poco::XML::XMLString & name = attributes.getLocalName(idx);
                const Poco::XML::XMLString & value = attributes.getValue(idx);
                attr[name] = value;
            }

            //
            // Create the element
            //
            ElementPtr element = ElementFactory::Instance().createElement(localName, parent, attr);
            if (element)
            {
                if (mStack.empty())
                {
                    assert(!mRootElement);
                    mRootElement = element;
                }
                mStack.push(element.get());
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
    }


    void Parser::endElement(const Poco::XML::XMLString& uri,
                            const Poco::XML::XMLString& localName,
                            const Poco::XML::XMLString& qname)
    {
        if (mIgnores > 0)
        {
            mIgnores--;
            return;
        }

        if (!mIsOverlay || mOverlayWrappers > 2)
        {
            mStack.top()->init();
            mStack.pop();
        }
        else
        {
            mOverlayWrappers--;
        }
    }
    
    
    void Parser::characters(const Poco::XML::XMLChar ch[], int start, int length)
    {
        if (!mStack.empty())
        {
            std::string innerText = std::string(ch + start, length);
            mStack.top()->setInnerText(mStack.top()->innerText() + innerText);
        }
    }


    Poco::XML::InputSource* Parser::resolveEntity(const Poco::XML::XMLString* publicId, const Poco::XML::XMLString& systemId)
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


    void Parser::releaseInputSource(Poco::XML::InputSource* pSource)
    {
        Poco::XML::EntityResolverImpl entityResolverImpl;
        return entityResolverImpl.releaseInputSource(pSource);
    }


} // namespace XULWin
