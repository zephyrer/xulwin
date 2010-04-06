#ifndef XULPARSER_H_INCLUDED
#define XULPARSER_H_INCLUDED


#include "XULWin/AttributesMapping.h"
#include "Poco/DOM/Element.h"
#include "Poco/SAX/SAXParser.h"
#include "Poco/SAX/ContentHandler.h"
#include "Poco/SAX/EntityResolver.h"
#include "Poco/SAX/Locator.h"
#include "Poco/Exception.h"
#include <stack>
#include <string>


namespace XULWin
{

    class AbstractXULParser : public Poco::XML::SAXParser,
        public Poco::XML::ContentHandler,
        public Poco::XML::EntityResolver
    {
    public:
        AbstractXULParser();

        /**
         * Returns the root element of the parsed XUL doc.
         * In case of parsing overlays, this will be the
         */
        Poco::XML::Element * rootElement() const;

    protected:
        virtual Poco::XML::Element * getCurrentParentElement() = 0;

        /**
         * Create the element.
         * Return true if element creation has succeeded or skipped.
         * Return false if element creation has failed.
         */
        virtual bool createElement(const std::string & inLocalName,
                                   Poco::XML::Element * inParent,
                                   const AttributesMapping & inAttributes,
                                   Poco::XML::Element * & outElement) = 0;

        virtual void pushStack(Poco::XML::Element * inElement) = 0;

        virtual void popStack() = 0;

    protected:
        // ContentHandler
        virtual void setDocumentLocator(const Poco::XML::Locator * inLocator);

        virtual void startDocument();

        virtual void endDocument();

        virtual void startElement(const Poco::XML::XMLString & uri,
                                  const Poco::XML::XMLString & localName,
                                  const Poco::XML::XMLString & qname,
                                  const Poco::XML::Attributes & attributes);

        virtual void endElement(const Poco::XML::XMLString & uri,
                                const Poco::XML::XMLString & localName,
                                const Poco::XML::XMLString & qname);

        virtual void characters(const Poco::XML::XMLChar ch[], int start, int length);

        virtual void ignorableWhitespace(const Poco::XML::XMLChar ch[], int start, int length) {}

        virtual void processingInstruction(const Poco::XML::XMLString & target, const Poco::XML::XMLString & data) {}

        virtual void startPrefixMapping(const Poco::XML::XMLString & prefix, const Poco::XML::XMLString & uri) {}

        virtual void endPrefixMapping(const Poco::XML::XMLString & prefix) {}

        virtual void skippedEntity(const Poco::XML::XMLString & name) {}

        virtual Poco::XML::InputSource * resolveEntity(const Poco::XML::XMLString * publicId, const Poco::XML::XMLString & systemId);

        virtual void releaseInputSource(Poco::XML::InputSource * pSource);


    protected:
        // needed to know which one is the parent element
        std::stack<Poco::XML::Element *> mStack;

        // depth of ignoration
        int mIgnores;
        Poco::XML::Element * mRootElement;

    private:
        void getAttributes(const Poco::XML::Attributes & inXMLAttributes,
                           AttributesMapping & outXULAttributes);

        const Poco::XML::Locator * mLocator;
        std::string mLanguage;
    };


    class XULParser : public AbstractXULParser
    {
    public:

    protected:
        virtual Poco::XML::Element * getCurrentParentElement();

        virtual void pushStack(Poco::XML::Element * inElement);

        virtual void popStack();

        /**
         * Create the element.
         * Return true if element creation has succeeded or skipped.
         * Return false if element creation has failed.
         */
        bool createElement(const std::string & inLocalName,
                           Poco::XML::Element * inParent,
                           const AttributesMapping & inAttributes,
                           Poco::XML::Element * & outElement);

    };

} // namespace XULWin


#endif // XULPARSER_H_INCLUDED
