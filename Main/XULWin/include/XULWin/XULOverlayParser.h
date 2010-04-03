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
        XULOverlayParser(Element * inOverlayElement);

    protected:
        virtual Element * getCurrentParentElement();

        virtual void pushStack(ElementPtr inElement);

        virtual void popStack();

        /**
         * Create the element.
         * Return true if element creation was succesfully created, or skipped.
         * Return false if element creation has failed.
         */
        bool createElement(const std::string & inLocalName,
                           Element * inParent,
                           const AttributesMapping & inAttributes,
                           ElementPtr & outElement);

    private:
        Element * mOverlayRoot;
    };


} // namespace XULWin


#endif // XULOVERLAYPARSER_H_INCLUDED
