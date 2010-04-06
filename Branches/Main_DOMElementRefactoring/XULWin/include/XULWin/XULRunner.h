#ifndef XULRUNNER_H_INCLUDED
#define XULRUNNER_H_INCLUDED


#include "XULWin/Fallible.h"
#include "XULWin/Windows.h"
#include "Poco/AutoPtr.h"
#include "Poco/DOM/Document.h"
#include <boost/scoped_ptr.hpp>
#include <string>


namespace XULWin
{

    class XULRunner
    {
    public:
        // Gets the previously set locale.
        // If no locale has been set en-US is returned.
        static std::string GetLocale();

        // Sets the desired locale (en-US, fr, nl, ...)
        // NOTE: the locale statements in the chrome.manifest file
        // are ignored, instead we depend on the foldernames in the chrome/locale directory.
        // This means that the locale value should be identical to its foldername.
        static void SetLocale(const std::string & inLocale);

        static HMODULE GetModuleHandle();

        static void SetModuleHandle(HMODULE inModuleHandle);

        XULRunner();

        /**
         * document
         *
         * Returns the DOM document object.
         * Requires parse first.
         */
        Poco::XML::Document * document();

        /**
         * loadApplication
         *
         * Loads a XUL application:
         *
         *  - Finds out which is the main XUL file. The location of the main XUL file
         *    must be specified in the file prefs.js which is found in the
         *    "<Application>/defaults/preferences" folder.
         *  - Parses the main XUL file.
         *  - Returns the root element of the parsed XUL file.
         */
        Poco::XML::Element * loadApplication(const std::string & inApplicationIniFile);

        /**
         * loadXULFromFile
         *
         * Load a XUL file:
         *
         *  - Finds the requested XUL file. This requires the
         *    chrome url to be translated to a native path.
         *  - Parses the XUL file.
         *  - Returns the root element of the parsed XUL file.
         */
        Poco::XML::Element * loadXULFromFile(const std::string & inXULUrl);


        /**
         * loadXULFromString
         *
         * Parses a XUL String.
         */
        Poco::XML::Element * loadXULFromString(const std::string & inXULString);


        /**
         * loadOverlay
         *
         * Loads a XUL overlay.
         *
         * The parameter is a chrome url to the overlay XUL document.
         * This document must have a <overlay> element as root element.
         * See the Mozilla documention on XUL overlays for more information.
         */
        void loadOverlay(const std::string & inXULUrl);

        Poco::XML::Element * rootElement() const;

    private:
        static Fallible<std::string> sLocale;
        static HMODULE sModuleHandle;

        Poco::XML::Element * parseFile(const std::string & inFilePath);
        Poco::XML::Element * parseString(const std::string & inXULString);
        std::string getOverlayElementId(const std::string & inXULURL);

        HMODULE mModuleHandle;
        Poco::XML::Element * mRootElement;
        Poco::AutoPtr<Poco::XML::Document> mDocument;
    };

} // XULWin


#endif // XULRUNNER_H_INCLUDED
