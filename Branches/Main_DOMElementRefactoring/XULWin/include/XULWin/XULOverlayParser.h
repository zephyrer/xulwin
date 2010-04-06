#ifndef XULOVERLAYPARSER_H_INCLUDED
#define XULOVERLAYPARSER_H_INCLUDED


#include "XULWin/XULParser.h"


namespace XULWin
{


    /**
     * XULOverlayParser
     *
     * This class is used for parsing XUL overlays.
     * See the XULRunner::loadOverlay method.
     */
    class XULOverlayParser : public AbstractXULParser
    {
    public:
        XULOverlayParser(Poco::XML::Element * inOverlayElement);

    protected:
        virtual Poco::XML::Element * getCurrentParentElement();

        virtual void pushStack(Poco::XML::Element * inElement);

        virtual void popStack();

        /**
         * Create the element.
         * Return true if element creation was succesfully created, or skipped.
         * Return false if element creation has failed.
         */
        bool createElement(const std::string & inLocalName,
                           Poco::XML::Element * inParent,
                           const AttributesMapping & inAttributes,
                           Poco::XML::Element * & outElement);

    private:
        Poco::XML::Element * mOverlayRoot;
    };


} // namespace XULWin


#endif // XULOVERLAYPARSER_H_INCLUDED
